#include "JSystem/JAudio/System/JASOscillator.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "types.h"

const f32 JASOscillator::sCurveTableLinear[17] = {
    1.0, 0.9375, 0.875, 0.8125, 0.75, 0.6875, 0.625, 0.5625, 0.5,
    0.4375, 0.375, 0.3125, 0.25, 0.1875, 0.125, 0.0625, 0.0,
};

const f32 JASOscillator::sCurveTableSampleCell[17] = {
    1.0, 0.9704890251159668, 0.7812740206718445, 0.5462809801101685, 0.39979198575019836, 
    0.28931498527526855, 0.21210399270057678, 0.15747599303722382, 0.1126129999756813, 0.08178959786891937, 
    0.057985201478004456, 0.04364150017499924, 0.03082370012998581, 0.023712899535894394, 0.015259300358593464, 
    0.00915555004030466, 0.0
};

const f32 JASOscillator::sCurveTableSqRoot[17] = {
    1.0, 0.8789060115814209, 0.765625, 0.6601560115814209, 0.5625, 0.4726560115814209,
    0.390625, 0.3164060115814209, 0.25, 0.1914059966802597, 0.140625, 0.09765619784593582,
    0.0625, 0.03515620157122612, 0.015625, 0.00390625, 0.0
};

const f32 JASOscillator::sCurveTableSquare[17] = {
    1.0, 0.9682459831237793, 0.9354140162467957, 0.9013879895210266, 0.8660249710083008,
    0.8291559815406799, 0.790569007396698, 0.75, 0.7071070075035095, 0.66143798828125,
    0.6123719811439514, 0.55901700258255, 0.5, 0.433012992143631, 0.35355299711227417, 0.25, 0.0,
};


JASOscillator::JASOscillator() {
    mData = NULL;
    _14 = 0;
    mDirectRelease = 0;
    _18 = 0;
    _1C = 0;
    
    _04 = _08 = _10 = _0C = 0.0f;
}

void JASOscillator::initStart(const Data *pData) {
    mData = pData;
    _04 = 0.0f;
    _08 = 0.0f;
    _0C = 0.0f;
    _14 = 0;
    mDirectRelease = 0;

    if (pData == NULL) {
        _1C = 0;
    }
    else if (mData->mTable == NULL) {
        _1C = 2;
        _08 = 1.0f;
    }
    else {
        _10 =  mData->mTable->_4 / 32768.0f;
        _18 = mData->mTable->_0;
        _1C = 1;
    }

}

void JASOscillator::incCounter(f32 x) {
    switch (_1C) {
    case 0:
    case 2:
        return;
    case 1: {
        break;
    }
    }
    _04 += x * mData->_04;
    update();
}

f32 JASOscillator::getValue() const {
#line 120
    JUT_ASSERT(mData);
    return mData->_14 + _08 * mData->mScale;
}

void JASOscillator::release() {
	if (mData->rel_table == NULL && mDirectRelease == 0) {
        stop();
	}

	if (mDirectRelease != 0) {
		_04 = 0.0f;
        _0C = _08;
        _10 = 0.0f;
        _14 = 0;
        _18 = (mDirectRelease >> 14) & 3;
        _1C = 4;
        update();
        return;
	}
	
    if (mData->mTable != mData->rel_table) {
#line 154
        JUT_ASSERT(mData->rel_table != 0);
        _04 = 0.0f;
        _0C = _08;
        _10 = mData->rel_table[0]._4 / 32768.0f;
        _14 = 0;
        _18 = mData->rel_table[0]._0;
    }

    _1C = 3;
    update();
}

void JASOscillator::update() {
    if (_1C == 4) {
        s16 x = mDirectRelease & 0x3FFF;
        if (_04 < x) {
            updateCurrentValue(x);
        } else {
            _08 = _10;
            _1C = 0;
        }
        return;
    }

    if (_1C == 2) return;

    const Point *table;
    if (_1C == 1) {
        table = mData->mTable;
    } else {
        table = mData->rel_table;
    }

    if (table == NULL) {
        _1C = 2;
        return;
    }
    
    while (_04 >= table[_14]._2) {
        _04 -= table[_14]._2;
        _08 = _10;
        _14++;
        _0C = _08;
        const s16 *ps = &table[_14]._0;
        switch(ps[0]) {
        case 0xf:
            _1C = 0;
            return;
        case 0xe:
            _1C = 2;
            return;
        case 0xd:
            _14 = ps[2];
            break;
        default:
            _18 = ps[0];
            _10 = ps[2] / 32768.0f;
            break;
        }
    }

    updateCurrentValue(table[_14]._2);
}

void JASOscillator::updateCurrentValue(f32 x) {
    static const f32 *table_list[4] = {
        JASOscillator::sCurveTableLinear,
        JASOscillator::sCurveTableSquare,
        JASOscillator::sCurveTableSqRoot,
        JASOscillator::sCurveTableSampleCell,
    };
    const f32 *table = table_list[_18];
    f32 fVar1 = 16.0f * (_04 / x);
    u32 index = (u32) fVar1;
    f32 fVar3 = (fVar1 - index);
    f32 fVar4 = (1.0f - fVar3) * table[index] + fVar3 * table[index + 1];
    _08 = _0C * fVar4 + _10 * (1.0f - fVar4);
}
