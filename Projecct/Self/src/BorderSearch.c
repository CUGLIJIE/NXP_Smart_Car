#include "BorderSearch.h"


bool LeftBorderSearchFrom(int16_t row, int16_t startIndex) {
    for(int16_t j = startIndex; j >= 0; --j) {
        if(image_binary[row][j]==0x00) {
            resultSet.leftBorder[row] = j;
            return TRUE;
        }
    }
    resultSet.leftBorder[row] = 0;
    ++resultSet.leftBorderNotFoundCnt;
    return FALSE;
}

bool RightBorderSearchFrom(int16_t row, int16_t startIndex) {
    for(int16_t j = startIndex; j < COL; ++j) {
        if(image_binary[row][j]==0x00) {
            resultSet.rightBorder[row] = j;
            return TRUE;
        }
    }
    resultSet.rightBorder[row] = COL;
    ++resultSet.rightBorderNotFoundCnt;
    return FALSE;
}

void MiddleLineUpdate(int16_t row) {
    resultSet.middleLine[row] = (resultSet.leftBorder[row] + resultSet.rightBorder[row]) / 2;
}

void MiddleLineUpdateAll() {
    for(int row = 0; row < ROW; ++row) {
        MiddleLineUpdate(row);
    }
}
