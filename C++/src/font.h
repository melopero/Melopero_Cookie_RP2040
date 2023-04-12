#include <cstdint>

const uint8_t glyphs[][5] = {
            {0,0,0,0,0},  // " ":    dec 32 - hex 0x20
            {0,29,0,0,0},  // "!": 
            {0,24,0,24,0},  // "\"":
            {10,31,10,31,10},  // "#": 
            {10,29,21,23,10},  // "$": 
            {25,18,4,9,19},  // "%": 
            {10,21,21,10,1},  // "&": 
            {0,24,0,0,0},  // "'": 
            {0,14,17,0,0},  // "(": 
            {0,17,14,0,0},  // ")": 
            {0,10,4,10,0},  // "*": 
            {0,4,14,4,0},  // "+": 
            {0,1,2,0,0},  // ",": 
            {0,4,4,4,0},  // "-": 
            {0,2,0,0,0},  // ".": 
            {1,2,4,8,16},  // "/": 
            {14,17,17,14,0},  // "0": 
            {0,9,31,1,0},  // "1": 
            {19,21,21,9,0},  // "2": 
            {18,17,21,26,0},  // "3": 
            {6,10,18,31,2},  // "4": 
            {29,21,21,21,18},  // "5": 
            {2,5,13,21,2},  // "6": 
            {17,18,20,24,16},  // "7": 
            {10,21,21,21,10},  // "8": 
            {8,21,22,20,8},  // "9": 
            {0,10,0,0,0},   // ":": 
            {0,1,10,0,0},   // ";": 
            {0,4,10,17,0},   // "<": 
            {0,10,10,10,0},   // "=": 
            {0,17,10,4,0},   // ">": 
            {8,16,21,20,8},   // "?": 
            {14,17,21,18,14},   // "@": 
            {15,20,20,15,0},  // "A": 
            {31,21,21,10,0},  // "B": 
            {14,17,17,17,0},  // "C": 
            {31,17,17,14,0},  // "D": 
            {31,21,21,17,0},  // "E": 
            {31,20,20,16,0},  // "F": 
            {14,17,17,21,6},  // "G": 
            {31,4,4,31,0},  // "H": 
            {17,31,17,0,0},  // "I": 
            {18,17,17,30,16},  // "J": 
            {31,4,10,17,0},  // "K": 
            {31,1,1,1,0},  // "L": 
            {31,8,4,8,31},  // "M": 
            {31,8,4,2,31},  // "N": 
            {14,17,17,14,0},  // "O": 
            {31,20,20,8,0},  // "P": 
            {12,18,19,13,0},  // "Q": 
            {31,20,20,10,1},  // "R": 
            {9,21,21,18,0},  // "S": 
            {16,16,31,16,16},  // "T": 
            {30,1,1,30,0},  // "U": 
            {28,2,1,2,28},  // "V": 
            {31,2,4,2,31},  // "W": 
            {27,4,4,27,0},  // "X": 
            {16,8,7,8,16},  // "Y": 
            {19,21,25,17,0},  // "Z": 
            {0,31,17,17,0},   // "[": 
            {16,8,4,2,1},    // "\\":
            {0,17,17,31,0},  // "]":
            {0,8,16,8,0},  // "^":
            {1,1,1,1,1},   // "_": 
            {0,16,8,0,0},   // "`": 
            {6,9,9,15,1},  // "a": 
            {31,5,5,2,0},  // "b": 
            {6,9,9,9,0},  // "c": 
            {2,5,5,31,0},  // "d": 
            {14,21,21,9,0},  // "e": 
            {4,15,20,16,0},  // "f": 
            {8,21,21,30,0},  // "g": 
            {31,4,4,3,0},  // "h": 
            {0,23,0,0,0},  // "i": 
            {0,1,1,22,0},  // "j": 
            {31,4,10,1,0},  // "k": 
            {0,30,1,1,0},  // "l": 
            {15,8,4,8,15},  // "m": 
            {15,8,8,7,0},  // "n": 
            {6,9,9,6,0},  // "o": 
            {15,10,10,4,0},  // "p": 
            {4,10,10,15,0},  // "q": 
            {7,8,8,8,0},  // "r": 
            {1,5,10,8,0},  // "s": 
            {0,30,5,5,1},  // "t": 
            {14,1,1,15,1},  // "u": 
            {12,2,1,2,12},  // "v": 
            {15,1,2,1,15},  // "w": 
            {9,6,6,9,0},  // "x": 
            {9,5,2,4,8},  // "y": 
            {9,11,13,9,0},  // "z": 
            {0,4,31,17,0},   // "{": 
            {0,31,0,0,0},   // "|": 
            {17,31,4,0,0},   // "}":   
            {0,4,4,2,2},   // "~":    dec 128 - hex 0x7E
            {4,8,31,8,4},  // '↑':
            {4,4,21,14,4},  // '→':
            {4,2,31,2,4},  // '↓':
            {4,14,21,4,4},  // '←':
            {2, 6, 14, 6, 2},  // '▲':
            {0, 31, 14, 4, 0},  // '►':
            {8, 12, 14, 12, 8},  // '▼':
            {0, 4, 14, 31, 0},  // '◄':
            {0, 14, 10, 14, },  // "☐":
            {31, 17, 17, 17, 31},  // "□":
            {24, 24, 7, 5, 5},// "℃":
            {24, 24, 7, 6, 4},     // "℉":
            {16, 31, 16, 31, 17},   //'π':
            {6,9,27,15,1},   //'å':
};