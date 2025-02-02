// 8 bytes
typedef struct _point{
    char x; // 4
    int y; // 4
}point;

// 5 bytes
typedef union _pt{
    char x; // 1
    int y; // 4
}pt;