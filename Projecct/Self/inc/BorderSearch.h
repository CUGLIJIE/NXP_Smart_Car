#ifndef __BORDER_SEARCH_RELATIVE
#define __BORDER_SEARCH_RELATIVE
#include "headfile.h"

bool LeftBorderSearchFrom(int16_t row, int16_t startIndex);
bool RightBorderSearchFrom(int16_t row, int16_t startIndex);
void MiddleLineUpdate(int16_t row);
void MiddleLineUpdateAll(void);

#define SHIFT                       3
#define MASK                        0x07
#define SetImgBufAsBitMap(row, col) (image_binary[row][(col) >> SHIFT] |=  (1 << ((col) & MASK)))
#define ClrImgBufAsBitMap(row, col) (image_binary[row][(col) >> SHIFT] &= ~(1 << ((col) & MASK)))
#define TstImgBufAsBitMap(row, col) (image_binary[row][(col) >> SHIFT] &   (1 << ((col) & MASK)))
#define IsWhite(row, col)           (image_binary[row][col])
#define IsBlack(row, col)           (!image_binary[row][col])


#endif
