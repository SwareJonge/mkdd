#include <string.h>
#include <dolphin/os.h>
#include <dolphin/gx/GXMisc.h>
#include "Osako/screenshot.h"

#define SCREENSHOT_BUFFER_SIZE (640 * 480 * 2)
#define SCREENSHOT_BUFFER_SIZE_RGB (640 * 480 * 3)
#define HIO_CHUNK_SIZE 640 * 2
#define HIO_BUFFER_SIZE (HIO_CHUNK_SIZE * 80)

BOOL g_connected;
BOOL g_ping_received;
u32 g_received_signal;
BOOL g_mail_waiting;
BOOL g_minimize_buffer;
u8 *g_data;

s32 g_usb_channel = -1;

static BOOL HostIOEnumCallback(s32 chan)
{
    g_usb_channel = chan;
    return FALSE;
}
static void HostIOCallback()
{
    g_mail_waiting = 1;
}

// Inline/Unused
static void CopyoutEFB(u8 *image_buffer, u32 width, u32 height)
{
    u16 x, y;
    u32 color;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++, image_buffer += 3)
        {
            GXPeekARGB(x, y, &color);
            image_buffer[2] = (u8)color;
            image_buffer[1] = (u8)(color >> 8);
            image_buffer[0] = (u8)(color >> 0x10);
        }
    }
}
static void CopyoutPortionEFB(u8 *image_buffer, u32 width, u32 startByte, u32 totalBytes)
{
    u32 color;
    u16 x = (startByte / 3) % width;
    u16 y = (startByte / 3) / width;
    u32 component = startByte % 3;
    u32 count = 0;

    while (count < totalBytes)
    {
        GXPeekARGB(x, y, &color);
        if (component == 0)
        {
            count++;
            image_buffer[0] = color >> 0x10;
        }

        if ((component <= 1) && (count < totalBytes))
        {
            count++;
            image_buffer[1 - component] = color >> 8;
        }

        if ((component <= 2) && (count < totalBytes))
        {
            count++;
            image_buffer[2 - component] = color;
        }

        image_buffer += (3 - component);
        component = 0;

        x++;
        if (width <= x)
        {
            x = 0;
            y++;
        }
    }
}

void TakeScreenshotEFB(SCREENSHOTAllocFunc);
void TakeScreenshotXFB(void *, SCREENSHOTAllocFunc);

static void WriteScreenshotPortionEFBtoUSB(u32 mail, SCREENSHOTAllocFunc allocFunc)
{
    u32 chunk;
    u32 totalBytes;
    if (g_data == NULL)
        g_data = (u8 *)allocFunc(0x400);

    for (chunk = 0; chunk < 100; chunk++)
    {
        CopyoutPortionEFB(g_data, 640, (mail * HIO_BUFFER_SIZE) + (chunk * 0x400), 0x400);
        DCFlushRange(g_data, 0x400);
        while (HIOWrite(0x500 + (chunk * 0x400), g_data, 0x400) == FALSE)
            ;
    }
}

static HostIOGrabtatus GrabChunk(u32 mail, void *bufferXFB, SCREENSHOTAllocFunc allocFunc, SCREENSHOTFreeFunc freeFunc)
{
    HostIOGrabtatus status;
    u32 color;
    u16 x, y;
    u32 chunk;
    u8 *fb;

    if ((mail >= 1 && mail < 7))
    {
        if (mail == 1 && g_minimize_buffer == FALSE)
        {
            if (g_data == NULL)
            {
                g_data = (u8 *)allocFunc(SCREENSHOT_BUFFER_SIZE);
            }
            memcpy(g_data, bufferXFB, SCREENSHOT_BUFFER_SIZE);
            DCFlushRange(g_data, SCREENSHOT_BUFFER_SIZE);
            OSReport("SCREENSHOT: Saved off screenshot to local memory.\n");
        }

        u8 *dst = (g_minimize_buffer) ? ((u8 *)bufferXFB) + (mail - 1) * HIO_BUFFER_SIZE : g_data + (mail - 1) * HIO_BUFFER_SIZE;
        while (HIOWrite(HIO_CHUNK_SIZE, dst, HIO_BUFFER_SIZE) == FALSE)
            ;

        OSReport("SCREENSHOT: Wrote chunk #%d.\n", mail);

        while (HIOWriteMailbox(mail) == FALSE)
            ;
        OSReport("SCREENSHOT: Notify of write chunk #%d put in mailbox.\n", g_received_signal);
    }
    else if (mail >= 0xb && mail < 0x14)
    {
        if (mail == 0xb && g_minimize_buffer == FALSE)
        {
            if (g_data == NULL)
            {
                g_data = (u8 *)allocFunc(SCREENSHOT_BUFFER_SIZE_RGB);
            }
            CopyoutEFB(g_data, 640, 480);

            DCFlushRange(g_data, SCREENSHOT_BUFFER_SIZE_RGB);
            OSReport("SCREENSHOT: Saved off screenshot to local memory.\n");
        }

        if (g_minimize_buffer)
        {
            WriteScreenshotPortionEFBtoUSB(mail - 0xb, allocFunc);
        }
        else
        {
            fb = g_data + (mail - 0xb) * HIO_BUFFER_SIZE;
            while (HIOWrite(0x500, fb, HIO_BUFFER_SIZE) == FALSE)
                ;
        }
        OSReport("SCREENSHOT: Wrote chunk #%d.\n", mail);

        while (HIOWriteMailbox(mail) == FALSE)
            ;
        OSReport("SCREENSHOT: Notify of write chunk #%d put in mailbox.\n", g_received_signal);
    }

    if ((mail == 6) || mail == 0x13)
    {
        if (g_data)
        {
            freeFunc(g_data);
            g_data = nullptr;
        }
        return GRAB_NOT_TRANSFERRING;
    }
    return GRAB_TRANSFERRING;
}

static void CheckMail(void *bufferXFB, SCREENSHOTAllocFunc allocFunc, SCREENSHOTFreeFunc freeFunc)
{
    u32 grabs = 0;
    HostIOGrabtatus status = GRAB_TRANSFERRING;
    u32 mail = 0;

    while (status == GRAB_TRANSFERRING)
    {
        grabs++;
        if (g_mail_waiting && HIOReadMailbox(&mail))
        {
            g_mail_waiting = FALSE;
            status = GRAB_NOT_TRANSFERRING;

            if (mail >= 0x14 && mail < 0x1d)
            {
                while (HIOWriteMailbox(mail) == NULL)
                    ;
                OSReport("SCREENSHOT: Sent ping back.\n");
            }
            else
            {
                if ((mail >= 1 && mail < 7) || (mail >= 0xb && mail < 0x14))
                {
                    g_received_signal = mail;
                    grabs = 0;
                    OSReport("SCREENSHOT: Grab screenshot request.\n");
                    status = GrabChunk(g_received_signal, bufferXFB, allocFunc, freeFunc);
                }
                else
                {
                    if (mail == 0x23)
                        g_minimize_buffer = TRUE;
                    else if (mail == 0x24)
                        g_minimize_buffer = FALSE;
                }
            }
        }

        if (g_minimize_buffer == FALSE)
            break;

        if (g_minimize_buffer && grabs > 150000000)
        {
            if (g_data)
            {
                freeFunc(g_data);
                g_data = nullptr;
            }
            break;
        }
    }
}

static BOOL ConnectToUSB()
{

    if (g_connected == FALSE)
    {
        if (HIOEnumDevices(HostIOEnumCallback) && g_usb_channel >= 0)
        {
            if (HIOInit(g_usb_channel, HostIOCallback))
            {
                u32 temp = 0;
                HIOReadMailbox(&temp);
                g_connected = TRUE;
                OSReport("SCREENSHOT: USB connected\n");
            }
        }
    }

    return g_connected;
}

void SCREENSHOTService(void *bufferXFB, SCREENSHOTAllocFunc allocFunc, SCREENSHOTFreeFunc freeFunc)
{
    if (g_connected == FALSE)
    {
        ConnectToUSB();
        return;
    }

    if (g_mail_waiting)
    {
        CheckMail(bufferXFB, allocFunc, freeFunc);
    }
}