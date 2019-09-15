
#include "utils.h"
#include "pcg_basic.h"

pcg32_random_t rng;

void mySrand()
{
	pcg32_srandom_r(&rng, time(nullptr) ^ (intptr_t)& printf,
		(intptr_t)& printf);
}

inline double sigmod(double in)
{
	return 1.0 / (1.0 + exp(-in));
}

inline uint32_t myRand()
{
	return pcg32_random_r(&rng);
}

inline double myRand_1to1()
{
	return ((int)(pcg32_random_r(&rng) & 0xffffff) - 0x7fffff) / ((double)0x7fffff);
}

inline double myRand_0to1()
{
	return ((int)(pcg32_random_r(&rng) & 0xffffff)) / ((double)0xffffff);
}

//半径30的周围相对坐标，近到远排列 共2922
// 5格及以内 < 95
//10格及以内 < 346
//15格及以内 < 754
//20格及以内 < 1320
//25格及以内 < 2042
//30格及以内 < 2922
int roundPoint[][2] = {
{1,0}, {0,1}, {0,-1}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}, {2,0}, {0,2},
{0,-2}, {-2,0}, {2,1}, {2,-1}, {1,2}, {1,-2}, {-1,2}, {-1,-2}, {-2,1}, {-2,-1},
{2,2}, {2,-2}, {-2,2}, {-2,-2}, {3,0}, {0,3}, {0,-3}, {-3,0}, {3,1}, {3,-1},
{1,3}, {1,-3}, {-1,3}, {-1,-3}, {-3,1}, {-3,-1}, {3,2}, {3,-2}, {2,3}, {2,-3},
{-2,3}, {-2,-3}, {-3,2}, {-3,-2}, {4,0}, {0,4}, {0,-4}, {-4,0}, {4,1}, {4,-1},
{1,4}, {1,-4}, {-1,4}, {-1,-4}, {-4,1}, {-4,-1}, {3,3}, {3,-3}, {-3,3}, {-3,-3},
{4,2}, {4,-2}, {2,4}, {2,-4}, {-2,4}, {-2,-4}, {-4,2}, {-4,-2}, {5,0}, {4,3},
{4,-3}, {3,4}, {3,-4}, {0,5}, {0,-5}, {-3,4}, {-3,-4}, {-4,3}, {-4,-3}, {-5,0},
{5,1}, {5,-1}, {1,5}, {1,-5}, {-1,5}, {-1,-5}, {-5,1}, {-5,-1}, {5,2}, {5,-2},
{2,5}, {2,-5}, {-2,5}, {-2,-5}, {-5,2}, {-5,-2}, {4,4}, {4,-4}, {-4,4}, {-4,-4},
{5,3}, {5,-3}, {3,5}, {3,-5}, {-3,5}, {-3,-5}, {-5,3}, {-5,-3}, {6,0}, {0,6},
{0,-6}, {-6,0}, {6,1}, {6,-1}, {1,6}, {1,-6}, {-1,6}, {-1,-6}, {-6,1}, {-6,-1},
{6,2}, {6,-2}, {2,6}, {2,-6}, {-2,6}, {-2,-6}, {-6,2}, {-6,-2}, {5,4}, {5,-4},
{4,5}, {4,-5}, {-4,5}, {-4,-5}, {-5,4}, {-5,-4}, {6,3}, {6,-3}, {3,6}, {3,-6},
{-3,6}, {-3,-6}, {-6,3}, {-6,-3}, {7,0}, {0,7}, {0,-7}, {-7,0}, {7,1}, {7,-1},
{5,5}, {5,-5}, {1,7}, {1,-7}, {-1,7}, {-1,-7}, {-5,5}, {-5,-5}, {-7,1}, {-7,-1},
{6,4}, {6,-4}, {4,6}, {4,-6}, {-4,6}, {-4,-6}, {-6,4}, {-6,-4}, {7,2}, {7,-2},
{2,7}, {2,-7}, {-2,7}, {-2,-7}, {-7,2}, {-7,-2}, {7,3}, {7,-3}, {3,7}, {3,-7},
{-3,7}, {-3,-7}, {-7,3}, {-7,-3}, {6,5}, {6,-5}, {5,6}, {5,-6}, {-5,6}, {-5,-6},
{-6,5}, {-6,-5}, {8,0}, {0,8}, {0,-8}, {-8,0}, {8,1}, {8,-1}, {7,4}, {7,-4},
{4,7}, {4,-7}, {1,8}, {1,-8}, {-1,8}, {-1,-8}, {-4,7}, {-4,-7}, {-7,4}, {-7,-4},
{-8,1}, {-8,-1}, {8,2}, {8,-2}, {2,8}, {2,-8}, {-2,8}, {-2,-8}, {-8,2}, {-8,-2},
{6,6}, {6,-6}, {-6,6}, {-6,-6}, {8,3}, {8,-3}, {3,8}, {3,-8}, {-3,8}, {-3,-8},
{-8,3}, {-8,-3}, {7,5}, {7,-5}, {5,7}, {5,-7}, {-5,7}, {-5,-7}, {-7,5}, {-7,-5},
{8,4}, {8,-4}, {4,8}, {4,-8}, {-4,8}, {-4,-8}, {-8,4}, {-8,-4}, {9,0}, {0,9},
{0,-9}, {-9,0}, {9,1}, {9,-1}, {1,9}, {1,-9}, {-1,9}, {-1,-9}, {-9,1}, {-9,-1},
{9,2}, {9,-2}, {7,6}, {7,-6}, {6,7}, {6,-7}, {2,9}, {2,-9}, {-2,9}, {-2,-9},
{-6,7}, {-6,-7}, {-7,6}, {-7,-6}, {-9,2}, {-9,-2}, {8,5}, {8,-5}, {5,8}, {5,-8},
{-5,8}, {-5,-8}, {-8,5}, {-8,-5}, {9,3}, {9,-3}, {3,9}, {3,-9}, {-3,9}, {-3,-9},
{-9,3}, {-9,-3}, {9,4}, {9,-4}, {4,9}, {4,-9}, {-4,9}, {-4,-9}, {-9,4}, {-9,-4},
{7,7}, {7,-7}, {-7,7}, {-7,-7}, {10,0}, {8,6}, {8,-6}, {6,8}, {6,-8}, {0,10},
{0,-10}, {-6,8}, {-6,-8}, {-8,6}, {-8,-6}, {-10,0}, {10,1}, {10,-1}, {1,10}, {1,-10},
{-1,10}, {-1,-10}, {-10,1}, {-10,-1}, {10,2}, {10,-2}, {2,10}, {2,-10}, {-2,10}, {-2,-10},
{-10,2}, {-10,-2}, {9,5}, {9,-5}, {5,9}, {5,-9}, {-5,9}, {-5,-9}, {-9,5}, {-9,-5},
{10,3}, {10,-3}, {3,10}, {3,-10}, {-3,10}, {-3,-10}, {-10,3}, {-10,-3}, {8,7}, {8,-7},
{7,8}, {7,-8}, {-7,8}, {-7,-8}, {-8,7}, {-8,-7}, {10,4}, {10,-4}, {4,10}, {4,-10},
{-4,10}, {-4,-10}, {-10,4}, {-10,-4}, {9,6}, {9,-6}, {6,9}, {6,-9}, {-6,9}, {-6,-9},
{-9,6}, {-9,-6}, {11,0}, {0,11}, {0,-11}, {-11,0}, {11,1}, {11,-1}, {1,11}, {1,-11},
{-1,11}, {-1,-11}, {-11,1}, {-11,-1}, {11,2}, {11,-2}, {10,5}, {10,-5}, {5,10}, {5,-10},
{2,11}, {2,-11}, {-2,11}, {-2,-11}, {-5,10}, {-5,-10}, {-10,5}, {-10,-5}, {-11,2}, {-11,-2},
{8,8}, {8,-8}, {-8,8}, {-8,-8}, {11,3}, {11,-3}, {9,7}, {9,-7}, {7,9}, {7,-9},
{3,11}, {3,-11}, {-3,11}, {-3,-11}, {-7,9}, {-7,-9}, {-9,7}, {-9,-7}, {-11,3}, {-11,-3},
{10,6}, {10,-6}, {6,10}, {6,-10}, {-6,10}, {-6,-10}, {-10,6}, {-10,-6}, {11,4}, {11,-4},
{4,11}, {4,-11}, {-4,11}, {-4,-11}, {-11,4}, {-11,-4}, {12,0}, {0,12}, {0,-12}, {-12,0},
{12,1}, {12,-1}, {9,8}, {9,-8}, {8,9}, {8,-9}, {1,12}, {1,-12}, {-1,12}, {-1,-12},
{-8,9}, {-8,-9}, {-9,8}, {-9,-8}, {-12,1}, {-12,-1}, {11,5}, {11,-5}, {5,11}, {5,-11},
{-5,11}, {-5,-11}, {-11,5}, {-11,-5}, {12,2}, {12,-2}, {2,12}, {2,-12}, {-2,12}, {-2,-12},
{-12,2}, {-12,-2}, {10,7}, {10,-7}, {7,10}, {7,-10}, {-7,10}, {-7,-10}, {-10,7}, {-10,-7},
{12,3}, {12,-3}, {3,12}, {3,-12}, {-3,12}, {-3,-12}, {-12,3}, {-12,-3}, {11,6}, {11,-6},
{6,11}, {6,-11}, {-6,11}, {-6,-11}, {-11,6}, {-11,-6}, {12,4}, {12,-4}, {4,12}, {4,-12},
{-4,12}, {-4,-12}, {-12,4}, {-12,-4}, {9,9}, {9,-9}, {-9,9}, {-9,-9}, {10,8}, {10,-8},
{8,10}, {8,-10}, {-8,10}, {-8,-10}, {-10,8}, {-10,-8}, {13,0}, {12,5}, {12,-5}, {5,12},
{5,-12}, {0,13}, {0,-13}, {-5,12}, {-5,-12}, {-12,5}, {-12,-5}, {-13,0}, {13,1}, {13,-1},
{11,7}, {11,-7}, {7,11}, {7,-11}, {1,13}, {1,-13}, {-1,13}, {-1,-13}, {-7,11}, {-7,-11},
{-11,7}, {-11,-7}, {-13,1}, {-13,-1}, {13,2}, {13,-2}, {2,13}, {2,-13}, {-2,13}, {-2,-13},
{-13,2}, {-13,-2}, {13,3}, {13,-3}, {3,13}, {3,-13}, {-3,13}, {-3,-13}, {-13,3}, {-13,-3},
{12,6}, {12,-6}, {6,12}, {6,-12}, {-6,12}, {-6,-12}, {-12,6}, {-12,-6}, {10,9}, {10,-9},
{9,10}, {9,-10}, {-9,10}, {-9,-10}, {-10,9}, {-10,-9}, {13,4}, {13,-4}, {11,8}, {11,-8},
{8,11}, {8,-11}, {4,13}, {4,-13}, {-4,13}, {-4,-13}, {-8,11}, {-8,-11}, {-11,8}, {-11,-8},
{-13,4}, {-13,-4}, {12,7}, {12,-7}, {7,12}, {7,-12}, {-7,12}, {-7,-12}, {-12,7}, {-12,-7},
{13,5}, {13,-5}, {5,13}, {5,-13}, {-5,13}, {-5,-13}, {-13,5}, {-13,-5}, {14,0}, {0,14},
{0,-14}, {-14,0}, {14,1}, {14,-1}, {1,14}, {1,-14}, {-1,14}, {-1,-14}, {-14,1}, {-14,-1},
{14,2}, {14,-2}, {10,10}, {10,-10}, {2,14}, {2,-14}, {-2,14}, {-2,-14}, {-10,10}, {-10,-10},
{-14,2}, {-14,-2}, {11,9}, {11,-9}, {9,11}, {9,-11}, {-9,11}, {-9,-11}, {-11,9}, {-11,-9},
{14,3}, {14,-3}, {13,6}, {13,-6}, {6,13}, {6,-13}, {3,14}, {3,-14}, {-3,14}, {-3,-14},
{-6,13}, {-6,-13}, {-13,6}, {-13,-6}, {-14,3}, {-14,-3}, {12,8}, {12,-8}, {8,12}, {8,-12},
{-8,12}, {-8,-12}, {-12,8}, {-12,-8}, {14,4}, {14,-4}, {4,14}, {4,-14}, {-4,14}, {-4,-14},
{-14,4}, {-14,-4}, {13,7}, {13,-7}, {7,13}, {7,-13}, {-7,13}, {-7,-13}, {-13,7}, {-13,-7},
{14,5}, {14,-5}, {11,10}, {11,-10}, {10,11}, {10,-11}, {5,14}, {5,-14}, {-5,14}, {-5,-14},
{-10,11}, {-10,-11}, {-11,10}, {-11,-10}, {-14,5}, {-14,-5}, {15,0}, {12,9}, {12,-9}, {9,12},
{9,-12}, {0,15}, {0,-15}, {-9,12}, {-9,-12}, {-12,9}, {-12,-9}, {-15,0}, {15,1}, {15,-1},
{1,15}, {1,-15}, {-1,15}, {-1,-15}, {-15,1}, {-15,-1}, {15,2}, {15,-2}, {2,15}, {2,-15},
{-2,15}, {-2,-15}, {-15,2}, {-15,-2}, {14,6}, {14,-6}, {6,14}, {6,-14}, {-6,14}, {-6,-14},
{-14,6}, {-14,-6}, {13,8}, {13,-8}, {8,13}, {8,-13}, {-8,13}, {-8,-13}, {-13,8}, {-13,-8},
{15,3}, {15,-3}, {3,15}, {3,-15}, {-3,15}, {-3,-15}, {-15,3}, {-15,-3}, {15,4}, {15,-4},
{4,15}, {4,-15}, {-4,15}, {-4,-15}, {-15,4}, {-15,-4}, {11,11}, {11,-11}, {-11,11}, {-11,-11},
{12,10}, {12,-10}, {10,12}, {10,-12}, {-10,12}, {-10,-12}, {-12,10}, {-12,-10}, {14,7}, {14,-7},
{7,14}, {7,-14}, {-7,14}, {-7,-14}, {-14,7}, {-14,-7}, {15,5}, {15,-5}, {13,9}, {13,-9},
{9,13}, {9,-13}, {5,15}, {5,-15}, {-5,15}, {-5,-15}, {-9,13}, {-9,-13}, {-13,9}, {-13,-9},
{-15,5}, {-15,-5}, {16,0}, {0,16}, {0,-16}, {-16,0}, {16,1}, {16,-1}, {1,16}, {1,-16},
{-1,16}, {-1,-16}, {-16,1}, {-16,-1}, {16,2}, {16,-2}, {14,8}, {14,-8}, {8,14}, {8,-14},
{2,16}, {2,-16}, {-2,16}, {-2,-16}, {-8,14}, {-8,-14}, {-14,8}, {-14,-8}, {-16,2}, {-16,-2},
{15,6}, {15,-6}, {6,15}, {6,-15}, {-6,15}, {-6,-15}, {-15,6}, {-15,-6}, {16,3}, {16,-3},
{12,11}, {12,-11}, {11,12}, {11,-12}, {3,16}, {3,-16}, {-3,16}, {-3,-16}, {-11,12}, {-11,-12},
{-12,11}, {-12,-11}, {-16,3}, {-16,-3}, {13,10}, {13,-10}, {10,13}, {10,-13}, {-10,13}, {-10,-13},
{-13,10}, {-13,-10}, {16,4}, {16,-4}, {4,16}, {4,-16}, {-4,16}, {-4,-16}, {-16,4}, {-16,-4},
{15,7}, {15,-7}, {7,15}, {7,-15}, {-7,15}, {-7,-15}, {-15,7}, {-15,-7}, {14,9}, {14,-9},
{9,14}, {9,-14}, {-9,14}, {-9,-14}, {-14,9}, {-14,-9}, {16,5}, {16,-5}, {5,16}, {5,-16},
{-5,16}, {-5,-16}, {-16,5}, {-16,-5}, {12,12}, {12,-12}, {-12,12}, {-12,-12}, {17,0}, {15,8},
{15,-8}, {8,15}, {8,-15}, {0,17}, {0,-17}, {-8,15}, {-8,-15}, {-15,8}, {-15,-8}, {-17,0},
{17,1}, {17,-1}, {13,11}, {13,-11}, {11,13}, {11,-13}, {1,17}, {1,-17}, {-1,17}, {-1,-17},
{-11,13}, {-11,-13}, {-13,11}, {-13,-11}, {-17,1}, {-17,-1}, {16,6}, {16,-6}, {6,16}, {6,-16},
{-6,16}, {-6,-16}, {-16,6}, {-16,-6}, {17,2}, {17,-2}, {2,17}, {2,-17}, {-2,17}, {-2,-17},
{-17,2}, {-17,-2}, {14,10}, {14,-10}, {10,14}, {10,-14}, {-10,14}, {-10,-14}, {-14,10}, {-14,-10},
{17,3}, {17,-3}, {3,17}, {3,-17}, {-3,17}, {-3,-17}, {-17,3}, {-17,-3}, {17,4}, {17,-4},
{16,7}, {16,-7}, {7,16}, {7,-16}, {4,17}, {4,-17}, {-4,17}, {-4,-17}, {-7,16}, {-7,-16},
{-16,7}, {-16,-7}, {-17,4}, {-17,-4}, {15,9}, {15,-9}, {9,15}, {9,-15}, {-9,15}, {-9,-15},
{-15,9}, {-15,-9}, {13,12}, {13,-12}, {12,13}, {12,-13}, {-12,13}, {-12,-13}, {-13,12}, {-13,-12},
{17,5}, {17,-5}, {5,17}, {5,-17}, {-5,17}, {-5,-17}, {-17,5}, {-17,-5}, {14,11}, {14,-11},
{11,14}, {11,-14}, {-11,14}, {-11,-14}, {-14,11}, {-14,-11}, {16,8}, {16,-8}, {8,16}, {8,-16},
{-8,16}, {-8,-16}, {-16,8}, {-16,-8}, {18,0}, {0,18}, {0,-18}, {-18,0}, {18,1}, {18,-1},
{17,6}, {17,-6}, {15,10}, {15,-10}, {10,15}, {10,-15}, {6,17}, {6,-17}, {1,18}, {1,-18},
{-1,18}, {-1,-18}, {-6,17}, {-6,-17}, {-10,15}, {-10,-15}, {-15,10}, {-15,-10}, {-17,6}, {-17,-6},
{-18,1}, {-18,-1}, {18,2}, {18,-2}, {2,18}, {2,-18}, {-2,18}, {-2,-18}, {-18,2}, {-18,-2},
{18,3}, {18,-3}, {3,18}, {3,-18}, {-3,18}, {-3,-18}, {-18,3}, {-18,-3}, {16,9}, {16,-9},
{9,16}, {9,-16}, {-9,16}, {-9,-16}, {-16,9}, {-16,-9}, {17,7}, {17,-7}, {13,13}, {13,-13},
{7,17}, {7,-17}, {-7,17}, {-7,-17}, {-13,13}, {-13,-13}, {-17,7}, {-17,-7}, {18,4}, {18,-4},
{14,12}, {14,-12}, {12,14}, {12,-14}, {4,18}, {4,-18}, {-4,18}, {-4,-18}, {-12,14}, {-12,-14},
{-14,12}, {-14,-12}, {-18,4}, {-18,-4}, {15,11}, {15,-11}, {11,15}, {11,-15}, {-11,15}, {-11,-15},
{-15,11}, {-15,-11}, {18,5}, {18,-5}, {5,18}, {5,-18}, {-5,18}, {-5,-18}, {-18,5}, {-18,-5},
{17,8}, {17,-8}, {8,17}, {8,-17}, {-8,17}, {-8,-17}, {-17,8}, {-17,-8}, {16,10}, {16,-10},
{10,16}, {10,-16}, {-10,16}, {-10,-16}, {-16,10}, {-16,-10}, {18,6}, {18,-6}, {6,18}, {6,-18},
{-6,18}, {-6,-18}, {-18,6}, {-18,-6}, {19,0}, {0,19}, {0,-19}, {-19,0}, {19,1}, {19,-1},
{1,19}, {1,-19}, {-1,19}, {-1,-19}, {-19,1}, {-19,-1}, {19,2}, {19,-2}, {14,13}, {14,-13},
{13,14}, {13,-14}, {2,19}, {2,-19}, {-2,19}, {-2,-19}, {-13,14}, {-13,-14}, {-14,13}, {-14,-13},
{-19,2}, {-19,-2}, {15,12}, {15,-12}, {12,15}, {12,-15}, {-12,15}, {-12,-15}, {-15,12}, {-15,-12},
{19,3}, {19,-3}, {17,9}, {17,-9}, {9,17}, {9,-17}, {3,19}, {3,-19}, {-3,19}, {-3,-19},
{-9,17}, {-9,-17}, {-17,9}, {-17,-9}, {-19,3}, {-19,-3}, {18,7}, {18,-7}, {7,18}, {7,-18},
{-7,18}, {-7,-18}, {-18,7}, {-18,-7}, {19,4}, {19,-4}, {16,11}, {16,-11}, {11,16}, {11,-16},
{4,19}, {4,-19}, {-4,19}, {-4,-19}, {-11,16}, {-11,-16}, {-16,11}, {-16,-11}, {-19,4}, {-19,-4},
{19,5}, {19,-5}, {5,19}, {5,-19}, {-5,19}, {-5,-19}, {-19,5}, {-19,-5}, {18,8}, {18,-8},
{8,18}, {8,-18}, {-8,18}, {-8,-18}, {-18,8}, {-18,-8}, {17,10}, {17,-10}, {10,17}, {10,-17},
{-10,17}, {-10,-17}, {-17,10}, {-17,-10}, {14,14}, {14,-14}, {-14,14}, {-14,-14}, {15,13}, {15,-13},
{13,15}, {13,-15}, {-13,15}, {-13,-15}, {-15,13}, {-15,-13}, {19,6}, {19,-6}, {6,19}, {6,-19},
{-6,19}, {-6,-19}, {-19,6}, {-19,-6}, {20,0}, {16,12}, {16,-12}, {12,16}, {12,-16}, {0,20},
{0,-20}, {-12,16}, {-12,-16}, {-16,12}, {-16,-12}, {-20,0}, {20,1}, {20,-1}, {1,20}, {1,-20},
{-1,20}, {-1,-20}, {-20,1}, {-20,-1}, {20,2}, {20,-2}, {2,20}, {2,-20}, {-2,20}, {-2,-20},
{-20,2}, {-20,-2}, {18,9}, {18,-9}, {9,18}, {9,-18}, {-9,18}, {-9,-18}, {-18,9}, {-18,-9},
{20,3}, {20,-3}, {3,20}, {3,-20}, {-3,20}, {-3,-20}, {-20,3}, {-20,-3}, {19,7}, {19,-7},
{17,11}, {17,-11}, {11,17}, {11,-17}, {7,19}, {7,-19}, {-7,19}, {-7,-19}, {-11,17}, {-11,-17},
{-17,11}, {-17,-11}, {-19,7}, {-19,-7}, {20,4}, {20,-4}, {4,20}, {4,-20}, {-4,20}, {-4,-20},
{-20,4}, {-20,-4}, {15,14}, {15,-14}, {14,15}, {14,-15}, {-14,15}, {-14,-15}, {-15,14}, {-15,-14},
{18,10}, {18,-10}, {10,18}, {10,-18}, {-10,18}, {-10,-18}, {-18,10}, {-18,-10}, {20,5}, {20,-5},
{19,8}, {19,-8}, {16,13}, {16,-13}, {13,16}, {13,-16}, {8,19}, {8,-19}, {5,20}, {5,-20},
{-5,20}, {-5,-20}, {-8,19}, {-8,-19}, {-13,16}, {-13,-16}, {-16,13}, {-16,-13}, {-19,8}, {-19,-8},
{-20,5}, {-20,-5}, {17,12}, {17,-12}, {12,17}, {12,-17}, {-12,17}, {-12,-17}, {-17,12}, {-17,-12},
{20,6}, {20,-6}, {6,20}, {6,-20}, {-6,20}, {-6,-20}, {-20,6}, {-20,-6}, {21,0}, {0,21},
{0,-21}, {-21,0}, {21,1}, {21,-1}, {19,9}, {19,-9}, {9,19}, {9,-19}, {1,21}, {1,-21},
{-1,21}, {-1,-21}, {-9,19}, {-9,-19}, {-19,9}, {-19,-9}, {-21,1}, {-21,-1}, {21,2}, {21,-2},
{18,11}, {18,-11}, {11,18}, {11,-18}, {2,21}, {2,-21}, {-2,21}, {-2,-21}, {-11,18}, {-11,-18},
{-18,11}, {-18,-11}, {-21,2}, {-21,-2}, {20,7}, {20,-7}, {7,20}, {7,-20}, {-7,20}, {-7,-20},
{-20,7}, {-20,-7}, {21,3}, {21,-3}, {15,15}, {15,-15}, {3,21}, {3,-21}, {-3,21}, {-3,-21},
{-15,15}, {-15,-15}, {-21,3}, {-21,-3}, {16,14}, {16,-14}, {14,16}, {14,-16}, {-14,16}, {-14,-16},
{-16,14}, {-16,-14}, {21,4}, {21,-4}, {4,21}, {4,-21}, {-4,21}, {-4,-21}, {-21,4}, {-21,-4},
{17,13}, {17,-13}, {13,17}, {13,-17}, {-13,17}, {-13,-17}, {-17,13}, {-17,-13}, {19,10}, {19,-10},
{10,19}, {10,-19}, {-10,19}, {-10,-19}, {-19,10}, {-19,-10}, {20,8}, {20,-8}, {8,20}, {8,-20},
{-8,20}, {-8,-20}, {-20,8}, {-20,-8}, {21,5}, {21,-5}, {5,21}, {5,-21}, {-5,21}, {-5,-21},
{-21,5}, {-21,-5}, {18,12}, {18,-12}, {12,18}, {12,-18}, {-12,18}, {-12,-18}, {-18,12}, {-18,-12},
{21,6}, {21,-6}, {6,21}, {6,-21}, {-6,21}, {-6,-21}, {-21,6}, {-21,-6}, {20,9}, {20,-9},
{16,15}, {16,-15}, {15,16}, {15,-16}, {9,20}, {9,-20}, {-9,20}, {-9,-20}, {-15,16}, {-15,-16},
{-16,15}, {-16,-15}, {-20,9}, {-20,-9}, {19,11}, {19,-11}, {11,19}, {11,-19}, {-11,19}, {-11,-19},
{-19,11}, {-19,-11}, {22,0}, {0,22}, {0,-22}, {-22,0}, {22,1}, {22,-1}, {17,14}, {17,-14},
{14,17}, {14,-17}, {1,22}, {1,-22}, {-1,22}, {-1,-22}, {-14,17}, {-14,-17}, {-17,14}, {-17,-14},
{-22,1}, {-22,-1}, {22,2}, {22,-2}, {2,22}, {2,-22}, {-2,22}, {-2,-22}, {-22,2}, {-22,-2},
{21,7}, {21,-7}, {7,21}, {7,-21}, {-7,21}, {-7,-21}, {-21,7}, {-21,-7}, {22,3}, {22,-3},
{18,13}, {18,-13}, {13,18}, {13,-18}, {3,22}, {3,-22}, {-3,22}, {-3,-22}, {-13,18}, {-13,-18},
{-18,13}, {-18,-13}, {-22,3}, {-22,-3}, {22,4}, {22,-4}, {20,10}, {20,-10}, {10,20}, {10,-20},
{4,22}, {4,-22}, {-4,22}, {-4,-22}, {-10,20}, {-10,-20}, {-20,10}, {-20,-10}, {-22,4}, {-22,-4},
{21,8}, {21,-8}, {19,12}, {19,-12}, {12,19}, {12,-19}, {8,21}, {8,-21}, {-8,21}, {-8,-21},
{-12,19}, {-12,-19}, {-19,12}, {-19,-12}, {-21,8}, {-21,-8}, {22,5}, {22,-5}, {5,22}, {5,-22},
{-5,22}, {-5,-22}, {-22,5}, {-22,-5}, {16,16}, {16,-16}, {-16,16}, {-16,-16}, {17,15}, {17,-15},
{15,17}, {15,-17}, {-15,17}, {-15,-17}, {-17,15}, {-17,-15}, {22,6}, {22,-6}, {18,14}, {18,-14},
{14,18}, {14,-18}, {6,22}, {6,-22}, {-6,22}, {-6,-22}, {-14,18}, {-14,-18}, {-18,14}, {-18,-14},
{-22,6}, {-22,-6}, {20,11}, {20,-11}, {11,20}, {11,-20}, {-11,20}, {-11,-20}, {-20,11}, {-20,-11},
{21,9}, {21,-9}, {9,21}, {9,-21}, {-9,21}, {-9,-21}, {-21,9}, {-21,-9}, {23,0}, {0,23},
{0,-23}, {-23,0}, {23,1}, {23,-1}, {19,13}, {19,-13}, {13,19}, {13,-19}, {1,23}, {1,-23},
{-1,23}, {-1,-23}, {-13,19}, {-13,-19}, {-19,13}, {-19,-13}, {-23,1}, {-23,-1}, {23,2}, {23,-2},
{22,7}, {22,-7}, {7,22}, {7,-22}, {2,23}, {2,-23}, {-2,23}, {-2,-23}, {-7,22}, {-7,-22},
{-22,7}, {-22,-7}, {-23,2}, {-23,-2}, {23,3}, {23,-3}, {3,23}, {3,-23}, {-3,23}, {-3,-23},
{-23,3}, {-23,-3}, {21,10}, {21,-10}, {10,21}, {10,-21}, {-10,21}, {-10,-21}, {-21,10}, {-21,-10},
{20,12}, {20,-12}, {12,20}, {12,-20}, {-12,20}, {-12,-20}, {-20,12}, {-20,-12}, {23,4}, {23,-4},
{17,16}, {17,-16}, {16,17}, {16,-17}, {4,23}, {4,-23}, {-4,23}, {-4,-23}, {-16,17}, {-16,-17},
{-17,16}, {-17,-16}, {-23,4}, {-23,-4}, {22,8}, {22,-8}, {8,22}, {8,-22}, {-8,22}, {-8,-22},
{-22,8}, {-22,-8}, {18,15}, {18,-15}, {15,18}, {15,-18}, {-15,18}, {-15,-18}, {-18,15}, {-18,-15},
{23,5}, {23,-5}, {5,23}, {5,-23}, {-5,23}, {-5,-23}, {-23,5}, {-23,-5}, {19,14}, {19,-14},
{14,19}, {14,-19}, {-14,19}, {-14,-19}, {-19,14}, {-19,-14}, {21,11}, {21,-11}, {11,21}, {11,-21},
{-11,21}, {-11,-21}, {-21,11}, {-21,-11}, {23,6}, {23,-6}, {22,9}, {22,-9}, {9,22}, {9,-22},
{6,23}, {6,-23}, {-6,23}, {-6,-23}, {-9,22}, {-9,-22}, {-22,9}, {-22,-9}, {-23,6}, {-23,-6},
{20,13}, {20,-13}, {13,20}, {13,-20}, {-13,20}, {-13,-20}, {-20,13}, {-20,-13}, {24,0}, {0,24},
{0,-24}, {-24,0}, {24,1}, {24,-1}, {1,24}, {1,-24}, {-1,24}, {-1,-24}, {-24,1}, {-24,-1},
{23,7}, {23,-7}, {17,17}, {17,-17}, {7,23}, {7,-23}, {-7,23}, {-7,-23}, {-17,17}, {-17,-17},
{-23,7}, {-23,-7}, {24,2}, {24,-2}, {18,16}, {18,-16}, {16,18}, {16,-18}, {2,24}, {2,-24},
{-2,24}, {-2,-24}, {-16,18}, {-16,-18}, {-18,16}, {-18,-16}, {-24,2}, {-24,-2}, {22,10}, {22,-10},
{10,22}, {10,-22}, {-10,22}, {-10,-22}, {-22,10}, {-22,-10}, {24,3}, {24,-3}, {21,12}, {21,-12},
{12,21}, {12,-21}, {3,24}, {3,-24}, {-3,24}, {-3,-24}, {-12,21}, {-12,-21}, {-21,12}, {-21,-12},
{-24,3}, {-24,-3}, {19,15}, {19,-15}, {15,19}, {15,-19}, {-15,19}, {-15,-19}, {-19,15}, {-19,-15},
{24,4}, {24,-4}, {4,24}, {4,-24}, {-4,24}, {-4,-24}, {-24,4}, {-24,-4}, {23,8}, {23,-8},
{8,23}, {8,-23}, {-8,23}, {-8,-23}, {-23,8}, {-23,-8}, {20,14}, {20,-14}, {14,20}, {14,-20},
{-14,20}, {-14,-20}, {-20,14}, {-20,-14}, {24,5}, {24,-5}, {5,24}, {5,-24}, {-5,24}, {-5,-24},
{-24,5}, {-24,-5}, {22,11}, {22,-11}, {11,22}, {11,-22}, {-11,22}, {-11,-22}, {-22,11}, {-22,-11},
{23,9}, {23,-9}, {21,13}, {21,-13}, {13,21}, {13,-21}, {9,23}, {9,-23}, {-9,23}, {-9,-23},
{-13,21}, {-13,-21}, {-21,13}, {-21,-13}, {-23,9}, {-23,-9}, {24,6}, {24,-6}, {6,24}, {6,-24},
{-6,24}, {-6,-24}, {-24,6}, {-24,-6}, {18,17}, {18,-17}, {17,18}, {17,-18}, {-17,18}, {-17,-18},
{-18,17}, {-18,-17}, {19,16}, {19,-16}, {16,19}, {16,-19}, {-16,19}, {-16,-19}, {-19,16}, {-19,-16},
{25,0}, {24,7}, {24,-7}, {20,15}, {20,-15}, {15,20}, {15,-20}, {7,24}, {7,-24}, {0,25},
{0,-25}, {-7,24}, {-7,-24}, {-15,20}, {-15,-20}, {-20,15}, {-20,-15}, {-24,7}, {-24,-7}, {-25,0},
{25,1}, {25,-1}, {1,25}, {1,-25}, {-1,25}, {-1,-25}, {-25,1}, {-25,-1}, {22,12}, {22,-12},
{12,22}, {12,-22}, {-12,22}, {-12,-22}, {-22,12}, {-22,-12}, {25,2}, {25,-2}, {23,10}, {23,-10},
{10,23}, {10,-23}, {2,25}, {2,-25}, {-2,25}, {-2,-25}, {-10,23}, {-10,-23}, {-23,10}, {-23,-10},
{-25,2}, {-25,-2}, {25,3}, {25,-3}, {3,25}, {3,-25}, {-3,25}, {-3,-25}, {-25,3}, {-25,-3},
{21,14}, {21,-14}, {14,21}, {14,-21}, {-14,21}, {-14,-21}, {-21,14}, {-21,-14}, {24,8}, {24,-8},
{8,24}, {8,-24}, {-8,24}, {-8,-24}, {-24,8}, {-24,-8}, {25,4}, {25,-4}, {4,25}, {4,-25},
{-4,25}, {-4,-25}, {-25,4}, {-25,-4}, {18,18}, {18,-18}, {-18,18}, {-18,-18}, {25,5}, {25,-5},
{23,11}, {23,-11}, {19,17}, {19,-17}, {17,19}, {17,-19}, {11,23}, {11,-23}, {5,25}, {5,-25},
{-5,25}, {-5,-25}, {-11,23}, {-11,-23}, {-17,19}, {-17,-19}, {-19,17}, {-19,-17}, {-23,11}, {-23,-11},
{-25,5}, {-25,-5}, {22,13}, {22,-13}, {13,22}, {13,-22}, {-13,22}, {-13,-22}, {-22,13}, {-22,-13},
{20,16}, {20,-16}, {16,20}, {16,-20}, {-16,20}, {-16,-20}, {-20,16}, {-20,-16}, {24,9}, {24,-9},
{9,24}, {9,-24}, {-9,24}, {-9,-24}, {-24,9}, {-24,-9}, {25,6}, {25,-6}, {6,25}, {6,-25},
{-6,25}, {-6,-25}, {-25,6}, {-25,-6}, {21,15}, {21,-15}, {15,21}, {15,-21}, {-15,21}, {-15,-21},
{-21,15}, {-21,-15}, {23,12}, {23,-12}, {12,23}, {12,-23}, {-12,23}, {-12,-23}, {-23,12}, {-23,-12},
{25,7}, {25,-7}, {7,25}, {7,-25}, {-7,25}, {-7,-25}, {-25,7}, {-25,-7}, {26,0}, {24,10},
{24,-10}, {10,24}, {10,-24}, {0,26}, {0,-26}, {-10,24}, {-10,-24}, {-24,10}, {-24,-10}, {-26,0},
{26,1}, {26,-1}, {1,26}, {1,-26}, {-1,26}, {-1,-26}, {-26,1}, {-26,-1}, {26,2}, {26,-2},
{22,14}, {22,-14}, {14,22}, {14,-22}, {2,26}, {2,-26}, {-2,26}, {-2,-26}, {-14,22}, {-14,-22},
{-22,14}, {-22,-14}, {-26,2}, {-26,-2}, {26,3}, {26,-3}, {19,18}, {19,-18}, {18,19}, {18,-19},
{3,26}, {3,-26}, {-3,26}, {-3,-26}, {-18,19}, {-18,-19}, {-19,18}, {-19,-18}, {-26,3}, {-26,-3},
{25,8}, {25,-8}, {20,17}, {20,-17}, {17,20}, {17,-20}, {8,25}, {8,-25}, {-8,25}, {-8,-25},
{-17,20}, {-17,-20}, {-20,17}, {-20,-17}, {-25,8}, {-25,-8}, {26,4}, {26,-4}, {4,26}, {4,-26},
{-4,26}, {-4,-26}, {-26,4}, {-26,-4}, {24,11}, {24,-11}, {21,16}, {21,-16}, {16,21}, {16,-21},
{11,24}, {11,-24}, {-11,24}, {-11,-24}, {-16,21}, {-16,-21}, {-21,16}, {-21,-16}, {-24,11}, {-24,-11},
{23,13}, {23,-13}, {13,23}, {13,-23}, {-13,23}, {-13,-23}, {-23,13}, {-23,-13}, {26,5}, {26,-5},
{5,26}, {5,-26}, {-5,26}, {-5,-26}, {-26,5}, {-26,-5}, {25,9}, {25,-9}, {9,25}, {9,-25},
{-9,25}, {-9,-25}, {-25,9}, {-25,-9}, {22,15}, {22,-15}, {15,22}, {15,-22}, {-15,22}, {-15,-22},
{-22,15}, {-22,-15}, {26,6}, {26,-6}, {6,26}, {6,-26}, {-6,26}, {-6,-26}, {-26,6}, {-26,-6},
{24,12}, {24,-12}, {12,24}, {12,-24}, {-12,24}, {-12,-24}, {-24,12}, {-24,-12}, {19,19}, {19,-19},
{-19,19}, {-19,-19}, {20,18}, {20,-18}, {18,20}, {18,-20}, {-18,20}, {-18,-20}, {-20,18}, {-20,-18},
{26,7}, {26,-7}, {25,10}, {25,-10}, {23,14}, {23,-14}, {14,23}, {14,-23}, {10,25}, {10,-25},
{7,26}, {7,-26}, {-7,26}, {-7,-26}, {-10,25}, {-10,-25}, {-14,23}, {-14,-23}, {-23,14}, {-23,-14},
{-25,10}, {-25,-10}, {-26,7}, {-26,-7}, {27,0}, {0,27}, {0,-27}, {-27,0}, {27,1}, {27,-1},
{21,17}, {21,-17}, {17,21}, {17,-21}, {1,27}, {1,-27}, {-1,27}, {-1,-27}, {-17,21}, {-17,-21},
{-21,17}, {-21,-17}, {-27,1}, {-27,-1}, {27,2}, {27,-2}, {2,27}, {2,-27}, {-2,27}, {-2,-27},
{-27,2}, {-27,-2}, {27,3}, {27,-3}, {3,27}, {3,-27}, {-3,27}, {-3,-27}, {-27,3}, {-27,-3},
{26,8}, {26,-8}, {22,16}, {22,-16}, {16,22}, {16,-22}, {8,26}, {8,-26}, {-8,26}, {-8,-26},
{-16,22}, {-16,-22}, {-22,16}, {-22,-16}, {-26,8}, {-26,-8}, {27,4}, {27,-4}, {24,13}, {24,-13},
{13,24}, {13,-24}, {4,27}, {4,-27}, {-4,27}, {-4,-27}, {-13,24}, {-13,-24}, {-24,13}, {-24,-13},
{-27,4}, {-27,-4}, {25,11}, {25,-11}, {11,25}, {11,-25}, {-11,25}, {-11,-25}, {-25,11}, {-25,-11},
{27,5}, {27,-5}, {23,15}, {23,-15}, {15,23}, {15,-23}, {5,27}, {5,-27}, {-5,27}, {-5,-27},
{-15,23}, {-15,-23}, {-23,15}, {-23,-15}, {-27,5}, {-27,-5}, {26,9}, {26,-9}, {9,26}, {9,-26},
{-9,26}, {-9,-26}, {-26,9}, {-26,-9}, {20,19}, {20,-19}, {19,20}, {19,-20}, {-19,20}, {-19,-20},
{-20,19}, {-20,-19}, {27,6}, {27,-6}, {21,18}, {21,-18}, {18,21}, {18,-21}, {6,27}, {6,-27},
{-6,27}, {-6,-27}, {-18,21}, {-18,-21}, {-21,18}, {-21,-18}, {-27,6}, {-27,-6}, {25,12}, {25,-12},
{12,25}, {12,-25}, {-12,25}, {-12,-25}, {-25,12}, {-25,-12}, {24,14}, {24,-14}, {14,24}, {14,-24},
{-14,24}, {-14,-24}, {-24,14}, {-24,-14}, {22,17}, {22,-17}, {17,22}, {17,-22}, {-17,22}, {-17,-22},
{-22,17}, {-22,-17}, {26,10}, {26,-10}, {10,26}, {10,-26}, {-10,26}, {-10,-26}, {-26,10}, {-26,-10},
{27,7}, {27,-7}, {7,27}, {7,-27}, {-7,27}, {-7,-27}, {-27,7}, {-27,-7}, {28,0}, {0,28},
{0,-28}, {-28,0}, {28,1}, {28,-1}, {23,16}, {23,-16}, {16,23}, {16,-23}, {1,28}, {1,-28},
{-1,28}, {-1,-28}, {-16,23}, {-16,-23}, {-23,16}, {-23,-16}, {-28,1}, {-28,-1}, {28,2}, {28,-2},
{2,28}, {2,-28}, {-2,28}, {-2,-28}, {-28,2}, {-28,-2}, {28,3}, {28,-3}, {27,8}, {27,-8},
{8,27}, {8,-27}, {3,28}, {3,-28}, {-3,28}, {-3,-28}, {-8,27}, {-8,-27}, {-27,8}, {-27,-8},
{-28,3}, {-28,-3}, {25,13}, {25,-13}, {13,25}, {13,-25}, {-13,25}, {-13,-25}, {-25,13}, {-25,-13},
{26,11}, {26,-11}, {11,26}, {11,-26}, {-11,26}, {-11,-26}, {-26,11}, {-26,-11}, {28,4}, {28,-4},
{20,20}, {20,-20}, {4,28}, {4,-28}, {-4,28}, {-4,-28}, {-20,20}, {-20,-20}, {-28,4}, {-28,-4},
{24,15}, {24,-15}, {15,24}, {15,-24}, {-15,24}, {-15,-24}, {-24,15}, {-24,-15}, {21,19}, {21,-19},
{19,21}, {19,-21}, {-19,21}, {-19,-21}, {-21,19}, {-21,-19}, {22,18}, {22,-18}, {18,22}, {18,-22},
{-18,22}, {-18,-22}, {-22,18}, {-22,-18}, {28,5}, {28,-5}, {5,28}, {5,-28}, {-5,28}, {-5,-28},
{-28,5}, {-28,-5}, {27,9}, {27,-9}, {9,27}, {9,-27}, {-9,27}, {-9,-27}, {-27,9}, {-27,-9},
{23,17}, {23,-17}, {17,23}, {17,-23}, {-17,23}, {-17,-23}, {-23,17}, {-23,-17}, {28,6}, {28,-6},
{26,12}, {26,-12}, {12,26}, {12,-26}, {6,28}, {6,-28}, {-6,28}, {-6,-28}, {-12,26}, {-12,-26},
{-26,12}, {-26,-12}, {-28,6}, {-28,-6}, {25,14}, {25,-14}, {14,25}, {14,-25}, {-14,25}, {-14,-25},
{-25,14}, {-25,-14}, {27,10}, {27,-10}, {10,27}, {10,-27}, {-10,27}, {-10,-27}, {-27,10}, {-27,-10},
{24,16}, {24,-16}, {16,24}, {16,-24}, {-16,24}, {-16,-24}, {-24,16}, {-24,-16}, {28,7}, {28,-7},
{7,28}, {7,-28}, {-7,28}, {-7,-28}, {-28,7}, {-28,-7}, {29,0}, {21,20}, {21,-20}, {20,21},
{20,-21}, {0,29}, {0,-29}, {-20,21}, {-20,-21}, {-21,20}, {-21,-20}, {-29,0}, {29,1}, {29,-1},
{1,29}, {1,-29}, {-1,29}, {-1,-29}, {-29,1}, {-29,-1}, {29,2}, {29,-2}, {26,13}, {26,-13},
{22,19}, {22,-19}, {19,22}, {19,-22}, {13,26}, {13,-26}, {2,29}, {2,-29}, {-2,29}, {-2,-29},
{-13,26}, {-13,-26}, {-19,22}, {-19,-22}, {-22,19}, {-22,-19}, {-26,13}, {-26,-13}, {-29,2}, {-29,-2},
{28,8}, {28,-8}, {8,28}, {8,-28}, {-8,28}, {-8,-28}, {-28,8}, {-28,-8}, {29,3}, {29,-3},
{27,11}, {27,-11}, {25,15}, {25,-15}, {15,25}, {15,-25}, {11,27}, {11,-27}, {3,29}, {3,-29},
{-3,29}, {-3,-29}, {-11,27}, {-11,-27}, {-15,25}, {-15,-25}, {-25,15}, {-25,-15}, {-27,11}, {-27,-11},
{-29,3}, {-29,-3}, {23,18}, {23,-18}, {18,23}, {18,-23}, {-18,23}, {-18,-23}, {-23,18}, {-23,-18},
{29,4}, {29,-4}, {4,29}, {4,-29}, {-4,29}, {-4,-29}, {-29,4}, {-29,-4}, {28,9}, {28,-9},
{24,17}, {24,-17}, {17,24}, {17,-24}, {9,28}, {9,-28}, {-9,28}, {-9,-28}, {-17,24}, {-17,-24},
{-24,17}, {-24,-17}, {-28,9}, {-28,-9}, {29,5}, {29,-5}, {5,29}, {5,-29}, {-5,29}, {-5,-29},
{-29,5}, {-29,-5}, {26,14}, {26,-14}, {14,26}, {14,-26}, {-14,26}, {-14,-26}, {-26,14}, {-26,-14},
{27,12}, {27,-12}, {12,27}, {12,-27}, {-12,27}, {-12,-27}, {-27,12}, {-27,-12}, {29,6}, {29,-6},
{6,29}, {6,-29}, {-6,29}, {-6,-29}, {-29,6}, {-29,-6}, {25,16}, {25,-16}, {16,25}, {16,-25},
{-16,25}, {-16,-25}, {-25,16}, {-25,-16}, {21,21}, {21,-21}, {-21,21}, {-21,-21}, {28,10}, {28,-10},
{22,20}, {22,-20}, {20,22}, {20,-22}, {10,28}, {10,-28}, {-10,28}, {-10,-28}, {-20,22}, {-20,-22},
{-22,20}, {-22,-20}, {-28,10}, {-28,-10}, {29,7}, {29,-7}, {23,19}, {23,-19}, {19,23}, {19,-23},
{7,29}, {7,-29}, {-7,29}, {-7,-29}, {-19,23}, {-19,-23}, {-23,19}, {-23,-19}, {-29,7}, {-29,-7},
{27,13}, {27,-13}, {13,27}, {13,-27}, {-13,27}, {-13,-27}, {-27,13}, {-27,-13}, {30,0}, {24,18},
{24,-18}, {18,24}, {18,-24}, {0,30}, {0,-30}, {-18,24}, {-18,-24}, {-24,18}, {-24,-18}, {-30,0},
{30,1}, {30,-1}, {26,15}, {26,-15}, {15,26}, {15,-26}, {1,30}, {1,-30}, {-1,30}, {-1,-30},
{-15,26}, {-15,-26}, {-26,15}, {-26,-15}, {-30,1}, {-30,-1}, {30,2}, {30,-2}, {2,30}, {2,-30},
{-2,30}, {-2,-30}, {-30,2}, {-30,-2}, {29,8}, {29,-8}, {28,11}, {28,-11}, {11,28}, {11,-28},
{8,29}, {8,-29}, {-8,29}, {-8,-29}, {-11,28}, {-11,-28}, {-28,11}, {-28,-11}, {-29,8}, {-29,-8},
{30,3}, {30,-3}, {3,30}, {3,-30}, {-3,30}, {-3,-30}, {-30,3}, {-30,-3}, {25,17}, {25,-17},
{17,25}, {17,-25}, {-17,25}, {-17,-25}, {-25,17}, {-25,-17}, {30,4}, {30,-4}, {4,30}, {4,-30},
{-4,30}, {-4,-30}, {-30,4}, {-30,-4}, {29,9}, {29,-9}, {9,29}, {9,-29}, {-9,29}, {-9,-29},
{-29,9}, {-29,-9}, {30,5}, {30,-5}, {27,14}, {27,-14}, {22,21}, {22,-21}, {21,22}, {21,-22},
{14,27}, {14,-27}, {5,30}, {5,-30}, {-5,30}, {-5,-30}, {-14,27}, {-14,-27}, {-21,22}, {-21,-22},
{-22,21}, {-22,-21}, {-27,14}, {-27,-14}, {-30,5}, {-30,-5}, {28,12}, {28,-12}, {12,28}, {12,-28},
{-12,28}, {-12,-28},
};

