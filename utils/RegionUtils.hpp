#ifndef _BLASR_REGION_UTILS_HPP_
#define _BLASR_REGION_UTILS_HPP_

#include <algorithm>
#include "SMRTSequence.hpp"
#include "reads/ReadInterval.hpp"
#include "reads/RegionTable.hpp"

bool LookupHQRegion(int holeNumber, RegionTable &regionTable, 
    int &start, int &end, int &score);

template<typename T_Sequence>
bool MaskRead(T_Sequence &fastaRead, ZMWGroupEntry &zmwData,
    RegionTable &regionTable);


template<typename T_Sequence>
bool GetReadTrimCoordinates(T_Sequence &fastaRead,
	ZMWGroupEntry &zmwData,	RegionTable &regionTable,
	DNALength &readStart ,DNALength &readEnd, int &score);

template<typename T_Sequence>
bool TrimRead(T_Sequence &fastaRead, ZMWGroupEntry &zmwData,
    RegionTable &regionTable, T_Sequence &trimmedRead);


class CompareRegionIndicesByStart {
public:
	RegionTable *regionTablePtr;
	int operator()(const int a, const int b) const;
};

		
int SortRegionIndicesByStart(RegionTable &regionTable, 
    std::vector<int> &indices);

class OrderRegionsByReadStart {
public:
    int operator()(const ReadInterval &lhs, const ReadInterval &rhs) const;
};

int FindRegionIndices(unsigned int holeNumber, RegionTable *regionTablePtr,
    int &regionLowIndex, int &regionHighIndex);

int FindRegionIndices(SMRTSequence &read, RegionTable *regionTablePtr, 
    int &regionLowIndex, int &regionHighIndex);

//
// Collect region indices for either all region types, or just a few 
// specific region types.
//
int CollectRegionIndices(SMRTSequence &read, RegionTable &regionTable, 
    std::vector<int> &regionIndices, RegionType *regionTypes=NULL,
    int numRegionTypes = 0);


template<typename T_Sequence>
void CollectSubreadIntervals(T_Sequence &read, RegionTable *regionTablePtr, 
    std::vector<ReadInterval> &subreadIntervals, bool byAdapter=false);

// Get all adapter intervals of a ZMW.
// Input:
//   read - read.zmwData.holeNumber specifies the zmw.
//   regionTablePtr - a pointer to a region table.
// Output:
//   adapterIntervals - where to assign all adapter intervals of the zmw
template<typename T_Sequence>
void CollectAdapterIntervals(T_Sequence &read, RegionTable *regionTablePtr,
    std::vector<ReadInterval> &adapterIntervals);

// Given a vecotr of ReadInterval objects and their corresponding 
// directions, intersect each object with an interval 
// [hqStart, hqEnd), if there is no intersection or the intersected
// interval is less than minIntervalLength, remove this object and
// their corresponding directions; otherwise, replace this object 
// with the intersected interval and keep their directions. 
// Return index of the (left-most) longest subread interval in the
// updated vector.
int GetHighQualitySubreadsIntervals(
    std::vector<ReadInterval> & subreadIntervals, 
    std::vector<int> & subreadDirections, 
    int hqStart, int hqEnd, int minIntervalLength = 0);


// Given a vector of subreads and a vector of adapters, return
// index of the (left-most) longest subread which has both
// adapters before & after itself.
int GetLongestFullSubreadIndex(
    std::vector<ReadInterval> & subreadIntervals,
    std::vector<ReadInterval> & adapterIntervals);

// Create a vector of n directions consisting of interleaved 0 and 1s.
void CreateDirections(std::vector<int> & directions, const int & n);

// Flop all directions in the given vector, if flop is true.
void UpdateDirections(std::vector<int> & directions, bool flop = false);

#include "utils/RegionUtilsImpl.hpp"

#endif
