#include "Cuver.h"

static int16_t black_pt_row;
//判断弯道类型
int16_t WhichCurve() {
    int16_t row;
    bool leftCurve = FALSE;
    bool rightCurve = FALSE;
    int16_t cnt = 0;
    for (row = 5; row < ROW-20 && IsWhite(row, resultSet.middleLine[row]); ++row) { }
    if(row < ROW && !InRange(resultSet.middleLine[row], COL / 2 - 30, COL / 2 + 50)) {
        black_pt_row = row;
        for(; row >= 0; --row) {
            if(!resultSet.foundLeftBorder[row]) {
                leftCurve = TRUE;
                break;
            } else if(!resultSet.foundRightBorder[row]) {
                rightCurve = TRUE;
                break;
            }
        }
        if(leftCurve) {
            for(; row >= 0; --row) {
                if(!resultSet.foundLeftBorder[row] && resultSet.foundRightBorder[row]) {
                    ++cnt;
                    if(cnt > 5) {
                        return LeftCurve;
                    }
                }
            }
        } else if(rightCurve) {
            for(; row >= 0; --row) {
                if(!resultSet.foundRightBorder[row] && resultSet.foundLeftBorder[row]) {
                    ++cnt;
                    if(cnt > 5) {
                        return RightCurve;
                    }
                }
            }
        }
    }
    return Unknown;
}
//弯道补线
void LeftCurveAction() {
    for (int row_ = ROW - 1; row_ > black_pt_row; --row_) {
        resultSet.middleLine[row_] = 0;
    }
    for (int cnt = 0; cnt < 12; ++cnt) {
        resultSet.middleLine[black_pt_row - cnt] = cnt * resultSet.middleLine[black_pt_row - 12] / 12;
    }
}

void RightCurveAction() {
    for (int row_ = ROW - 1; row_ > black_pt_row; --row_) {
        resultSet.middleLine[row_] = COL - 1;
    }
    for (int cnt = 0; cnt < 12; ++cnt) {
        resultSet.middleLine[black_pt_row - cnt] =COL - 1
            - cnt * (COL - 1 - resultSet.middleLine[black_pt_row - 12]) / 12;
    }
}

