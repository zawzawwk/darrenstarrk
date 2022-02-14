/*
    File:       FormStyle.h

    Contains:   Default dot widget in vga mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

static int Icon[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x1f1c, //0,1,1,1,1,1,0,0,0,1,1,1,0,0
    0x1122, //0,1,0,0,0,1,0,0,1,0,0,0,1,0
    0x1122, //0,1,0,0,0,1,0,0,1,0,0,0,1,0
    0x1122, //0,1,0,0,0,1,0,0,1,0,0,0,1,0
    0x1f1c, //0,1,1,1,1,1,0,0,0,1,1,1,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x1f3e, //0,1,1,1,1,1,0,0,1,1,1,1,1,0
    0x1122, //0,1,0,0,0,1,0,0,1,0,0,0,1,0
    0x1122, //0,1,0,0,0,1,0,0,1,0,0,0,1,0
    0x1122, //0,1,0,0,0,1,0,0,1,0,0,0,1,0
    0x1f3e, //0,1,1,1,1,1,0,0,1,1,1,1,1,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int Min[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x1ff8, //0,0,1,1,1,1,1,1,1,1,1,1,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int Max[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0e1c, //0,0,1,1,1,0,0,0,0,1,1,1,0,0
    0x0c0c, //0,0,1,1,0,0,0,0,0,0,1,1,0,0
    0x0a14, //0,0,1,0,1,0,0,0,0,1,0,1,0,0
    0x0120, //0,0,0,0,0,1,0,0,1,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0120, //0,0,0,0,0,1,0,0,1,0,0,0,0,0
    0x0a14, //0,0,1,0,1,0,0,0,0,1,0,1,0,0
    0x0c0c, //0,0,1,1,0,0,0,0,0,0,1,1,0,0
    0x0e1c, //0,0,1,1,1,0,0,0,0,1,1,1,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int MaxBack[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0210, //0,0,0,0,1,0,0,0,0,1,0,0,0,0
    0x0528, //0,0,0,1,0,1,0,0,1,0,1,0,0,0
    0x0210, //0,0,0,0,1,0,0,0,0,1,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0210, //0,0,0,0,1,0,0,0,0,1,0,0,0,0
    0x0528, //0,0,0,1,0,1,0,0,1,0,1,0,0,0
    0x0210, //0,0,0,0,1,0,0,0,0,1,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int Normal[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0804, //0,0,1,0,0,0,0,0,0,0,0,1,0,0
    0x0528, //0,0,0,1,0,1,0,0,1,0,1,0,0,0
    0x0330, //0,0,0,0,1,1,0,0,1,1,0,0,0,0
    0x0738, //0,0,0,1,1,1,0,0,1,1,1,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0738, //0,0,0,1,1,1,0,0,1,1,1,0,0,0
    0x0330, //0,0,0,0,1,1,0,0,1,1,0,0,0,0
    0x0528, //0,0,0,1,0,1,0,0,1,0,1,0,0,0
    0x0804, //0,0,1,0,0,0,0,0,0,0,0,1,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int NormalBack[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0a14, //0,0,1,0,1,0,0,0,0,1,0,1,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0a14, //0,0,1,0,1,0,0,0,0,1,0,1,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int Exit[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0804, //0,0,1,0,0,0,0,0,0,0,0,1,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0210, //0,0,0,0,1,0,0,0,0,1,0,0,0,0
    0x0120, //0,0,0,0,0,1,0,0,1,0,0,0,0,0
    0x00C0, //0,0,0,0,0,0,1,1,0,0,0,0,0,0
    0x00C0, //0,0,0,0,0,0,1,1,0,0,0,0,0,0
    0x0120, //0,0,0,0,0,1,0,0,1,0,0,0,0,0
    0x0210, //0,0,0,0,1,0,0,0,0,1,0,0,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0804, //0,0,1,0,0,0,0,0,0,0,0,1,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int ExitBack[14] = {
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0a14, //0,0,1,0,1,0,0,0,0,1,0,1,0,0
    0x0528, //0,0,0,1,0,1,0,0,1,0,1,0,0,0
    0x02d0, //0,0,0,0,1,0,1,1,0,1,0,0,0,0
    0x0120, //0,0,0,0,0,1,0,0,1,0,0,0,0,0
    0x0120, //0,0,0,0,0,1,0,0,1,0,0,0,0,0
    0x02d0, //0,0,0,0,1,0,1,1,0,1,0,0,0,0
    0x0528, //0,0,0,1,0,1,0,0,1,0,1,0,0,0
    0x0a14, //0,0,1,0,1,0,0,0,0,1,0,1,0,0
    0x0408, //0,0,0,1,0,0,0,0,0,0,1,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
    0x0000, //0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#if 0
static unsigned char ShadowCorners_Right_Bottom[100] = {
    137, 127, 117, 97, 87, 67, 47, 27, 17, 7,
    127, 123, 113, 93, 83, 65, 43, 23, 15, 7,
    117, 113, 107, 90, 77, 63, 40, 22, 14, 5,
     97,  93,  90, 87, 73, 60, 37, 21, 13, 5,
     87,  83,  77, 73, 70, 53, 33, 20, 12, 5,
     67,  65,  63, 60, 53, 57, 27, 19, 11, 5,
     47,  43,  40, 37, 33, 27, 23, 17,  7, 0,
     27,  23,  22, 21, 20, 19, 10,  7,  0, 0,
     17,  15,  14, 13, 11, 11,  7,  0,  0, 0,
      7,   7,   5,  5,  5,  5,  0,  0,  0, 0,
};

static unsigned char ShadowCorners_Left_Bottom[100] = {
    7, 17, 27, 47, 67, 87, 97, 117, 127, 137,
    7, 15, 23, 43, 65, 83, 93, 113, 123, 127,
    5, 14, 22, 40, 63, 77, 90, 107, 113, 117,
    5, 13, 21, 37, 60, 73, 87,  90,  93,  97,
    5, 12, 20, 33, 53, 70, 73,  77,  83,  87,
    5, 11, 19, 27, 57, 53, 60,  63,  65,  67,
    0,  7, 17, 23, 27, 33, 37,  40,  43,  47,
    0,  0,  7, 10, 19, 20, 21,  22,  23,  27,
    0,  0,  0,  7, 11, 11, 13,  14,  15,  17,
    0,  0,  0,  0,  5,  5,  5,   5,   7,   7,
};

static unsigned char ShadowCorners_Right_Top[100] = {
      7,   7,   5,  5,  5,  5,  0,  0,  0, 0,
     17,  15,  14, 13, 11, 11,  7,  0,  0, 0,
     27,  23,  22, 21, 20, 19, 10,  7,  0, 0,
     47,  43,  40, 37, 33, 27, 23, 17,  7, 0,
     67,  65,  63, 60, 53, 57, 27, 19, 11, 5,
     87,  83,  77, 73, 70, 53, 33, 20, 12, 5,
     97,  93,  90, 87, 73, 60, 37, 21, 13, 5,
    117, 113, 107, 90, 77, 63, 40, 22, 14, 5,
    127, 123, 113, 93, 83, 65, 43, 23, 15, 7,
    137, 127, 117, 97, 87, 67, 47, 27, 17, 7,
};

static unsigned char ShadowCorners_Left_Top[100] = {
    0,  0,  0,  0,  5,  5,  5,   5,   7,   7,
    0,  0,  0,  7, 11, 11, 13,  14,  15,  17,
    0,  0,  7, 10, 19, 20, 21,  22,  23,  27,
    0,  7, 17, 23, 27, 33, 37,  40,  43,  47,
    5, 11, 19, 27, 57, 53, 60,  63,  65,  67,
    5, 12, 20, 33, 53, 70, 73,  77,  83,  87,
    5, 13, 21, 37, 60, 73, 87,  90,  93,  97,
    5, 14, 22, 40, 63, 77, 90, 107, 113, 117,
    7, 15, 23, 43, 65, 83, 93, 113, 123, 127,
    7, 17, 27, 47, 67, 87, 97, 117, 127, 137,
};
#endif
