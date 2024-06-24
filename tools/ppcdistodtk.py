ppcdisFile = open('config/MarioClub_us/dol_slices.yml', 'r')
Lines = ppcdisFile.readlines()

dtkFile = open('splits.txt', 'w')
newObjects = open('obj.txt', 'w')

# not entirely sure how dtk handles seperate libs so that has to be refactored most likely
replaceTbl = [
    "libs/", "",
    "src/", "", 
    ": [", " start:",
    ", ", " end:",
    "]", ""
]

for line in Lines:
    newline = line
    for i in range(0, len(replaceTbl), 2):
        newline = newline.replace(replaceTbl[i], replaceTbl[i + 1])
    
    if(line != newline): # only save the new line if it actually has replaced stuff
        match = "Matching"
        if(line.__contains__("#")):
            line = line.replace("#", "")
            match = "NonMatching"

        newline = newline.replace('#', '')
        dtkFile.write(newline)
        
        if f":" in line and (".c" in line or ".cpp" in line):
            pathEndIdx = newline.index(":")
            path = newline[:pathEndIdx]
            newObjects.write(f"    Object({match}, \"{path}\"),\n")

ppcdisFile.close()
dtkFile.close()
newObjects.close()
    