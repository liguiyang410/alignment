#include "ScoreMatrices.hpp"

int QVDistanceMatrix[5][5] = {
	{-1, 1, 1, 1, 1},
	{1, -1, 1, 1, 1},
	{1, 1, -1, 1, 1},
	{1, 1, 1, -1, 1},
	{1, 1, 1, 1, 1}
};

int EditDistanceMatrix[5][5] = {
	{0, 1, 1, 1, 1},
	{1, 0, 1, 1, 1},
	{1, 1, 0, 1, 1},
	{1, 1, 1, 0, 1},
	{1, 1, 1, 1, 1}
};

int SMRTDistanceMatrix[5][5] = {
	{-5, 6, 6, 6, 6},
	{6, -5, 6, 6, 6},
	{6, 6, -5, 6, 6},
	{6, 6, 6, -5, 6},
	{6, 6, 6, 6,  0}
};

int SMRTLogProbMatrix[5][5] = {
  {0, 15, 15, 15, 15},
  {15, 0, 15, 15, 15},
  {15, 15, 0, 15, 15},
  {15, 15, 15, 0, 15},
  {15, 15, 15, 15, 0},
};

int LowMutationMatrix[5][5] = {
	{0, 5, 5, 5, 5},
	{5, 0, 5, 5, 5},
	{5, 5, 0, 5, 5},
	{5, 5, 5, 0, 5},
	{5, 5, 5,  5, 5}
};

int LocalAlignLowMutationMatrix[5][5] = {
	{-2, 5, 5, 5, 5},
	{5, -2, 5, 5, 5},
	{5, 5, -2, 5, 5},
	{5, 5, 5, -2, 5},
	{5, 5, 5,  5, 5}
};

int CrossMatchMatrix[5][5] = {
	{-1, 2, 2, 2, 2},
	{2, -1, 2, 2, 2},
	{2, 2, -1, 2, 2},
	{2, 2, 2, -1, 2},
	{2, 2, 2,  2, 2}
};

