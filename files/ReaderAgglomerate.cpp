#include "files/ReaderAgglomerate.hpp"

void ReaderAgglomerate::SetToUpper() {
    fastaReader.SetToUpper();
}

void ReaderAgglomerate::InitializeParameters() {
    start  = 0;
    stride = 1;
    subsample = 1.1;
    readQuality = 1;
    useRegionTable = true;
    ignoreCCS = true;
}

ReaderAgglomerate::ReaderAgglomerate() {
    InitializeParameters();
}

ReaderAgglomerate::ReaderAgglomerate(float _subsample) {
    this->InitializeParameters();
    subsample = _subsample;
}

ReaderAgglomerate::ReaderAgglomerate(int _stride) {
    this->InitializeParameters();
    stride = _stride;
}

ReaderAgglomerate::ReaderAgglomerate(int _start, int _stride) {
    this->InitializeParameters();
    start  = _start;
    stride = _stride;
}

void ReaderAgglomerate::GetMovieName(string &movieName) {
    if (fileType == Fasta || fileType == Fastq) {
        movieName = fileName;
    }
    else if (fileType == HDFPulse || fileType == HDFBase ||
            fileType == HDFCCS || fileType == HDFCCSONLY) {
        movieName = hdfBasReader.GetMovieName();
    }
}

bool ReaderAgglomerate::FileHasZMWInformation() {
    return (fileType == HDFPulse || fileType == HDFBase || 
            fileType == HDFCCS || fileType == HDFCCSONLY);
}

void ReaderAgglomerate::SkipReadQuality() {
    readQuality = 0;
}

void ReaderAgglomerate::IgnoreCCS() {
    ignoreCCS = true;
}
  
void ReaderAgglomerate::UseCCS() {
    ignoreCCS = false;
    hdfBasReader.SetReadBasesFromCCS();
}

int ReaderAgglomerate::Initialize(string &pFileName) {
    if (DetermineFileTypeByExtension(pFileName, fileType)) {
        fileName = pFileName;
        return Initialize();
    }
    return false;
}

bool ReaderAgglomerate::SetReadFileName(string &pFileName) {
    if (DetermineFileTypeByExtension(pFileName, fileType)) {
        fileName = pFileName;
        return true;
    }
    else {
        return false;
    }
}

int ReaderAgglomerate::Initialize(FileType &pFileType, string &pFileName) {
    SetFiles(pFileType, pFileName);
    return Initialize();
}

bool ReaderAgglomerate::HasRegionTable() {
    switch(fileType) {
        case Fasta:
            return false;
            break;
        case Fastq:
            return false;
            break;
        case HDFPulse:
        case HDFBase:
            return hdfBasReader.HasRegionTable();
            break;
        case HDFCCSONLY:
        case HDFCCS:
            return hdfCcsReader.HasRegionTable();
            break;
    }
    return false;
}

int ReaderAgglomerate::Initialize() {
    int init = 1;
    switch(fileType) {
        case Fasta:
            init = fastaReader.Init(fileName);
            break;
        case Fastq:
            init = fastqReader.Init(fileName);
            break;
        case HDFCCSONLY:
            ignoreCCS = false;
            hdfCcsReader.SetReadBasesFromCCS();
            hdfCcsReader.InitializeDefaultIncludedFields();
            init = hdfCcsReader.Initialize(fileName);
            if (init == 0) return 0;
            break;
        case HDFPulse:
        case HDFBase:
            //
            // Here one needs to test and see if the hdf file contains ccs.
            // If this is the case, then the file type is HDFCCS.
            if (hdfCcsReader.BasFileHasCCS(fileName) and !ignoreCCS) {
                fileType = HDFCCS;
                hdfCcsReader.InitializeDefaultIncludedFields();
                init = hdfCcsReader.Initialize(fileName);
                if (init == 0) return 0;
            }
            else {
                hdfBasReader.InitializeDefaultIncludedFields();
                init = hdfBasReader.Initialize(fileName);
                //
                // This code is added so that meaningful names are printed 
                // when running on simulated data that contains the coordinate
                // information.
                if (init == 0) return 0;
            }
            break;
    }
    readGroupId = "";
    if (init == 0 || (start > 0 && Advance(start) == 0) ){
        return 0;
    };

    string movieName;
    GetMovieName(movieName);
    string moviePlusFileName = movieName + fileName;
    MakeMD5(moviePlusFileName, readGroupId, 10);

    return 1;
}

ReaderAgglomerate & ReaderAgglomerate::operator=(ReaderAgglomerate &rhs) {
    fileType     = rhs.fileType;
    fileName = rhs.fileName;
    return *this;
}

bool ReaderAgglomerate::Subsample(float rate) {
    bool retVal = true;
    while( (rand() % 100 + 1) > (rate * 100) and (retVal = Advance(1)));
    return retVal;
}

int ReaderAgglomerate::GetNext(FASTASequence &seq) {
    int numRecords = 0;
    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch(fileType) {
        case Fasta:
            numRecords = fastaReader.GetNext(seq);
            break;
        case Fastq:
            numRecords = fastqReader.GetNext(seq);
            break;
        case HDFPulse:
        case HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case HDFCCSONLY:
        case HDFCCS:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
    }
    seq.CleanupOnFree();
    return numRecords;
}

int ReaderAgglomerate::GetNext(FASTQSequence &seq) {
    int numRecords = 0;
    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch(fileType) {
        case Fasta:
            numRecords = fastaReader.GetNext(seq);
            break;
        case Fastq:
            numRecords = fastqReader.GetNext(seq);
            break;
        case HDFPulse:
        case HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case HDFCCSONLY:
        case HDFCCS:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
    }
    if (stride > 1)
        Advance(stride-1);
    return numRecords;
}

int ReaderAgglomerate::GetNext(SMRTSequence &seq) {
    int numRecords = 0;

    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch(fileType) {
        case Fasta:
            numRecords = fastaReader.GetNext(seq);
            break;
        case Fastq:
            numRecords = fastqReader.GetNext(seq);
            break;
        case HDFPulse:
        case HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case HDFCCSONLY:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
        case HDFCCS:
            assert(ignoreCCS == false);
            assert(hdfBasReader.readBasesFromCCS == true);
            numRecords = hdfBasReader.GetNext(seq);
            break;
    }
    if (stride > 1)
        Advance(stride-1);
    return numRecords;
}

int ReaderAgglomerate::GetNextBases(SMRTSequence &seq, bool readQVs) {
    int numRecords = 0;

    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch(fileType) {
        case Fasta:
            cout << "ERROR! Can only GetNextBases from a Pulse or Base File." << endl;
            assert(0);
            break;
        case Fastq:
            cout << "ERROR! Can only GetNextBases from a Pulse or Base File." << endl;
            assert(0);
            break;
        case HDFPulse:
        case HDFBase:
            numRecords = hdfBasReader.GetNextBases(seq, readQVs);
            break;
        case HDFCCSONLY:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
        case HDFCCS:
            cout << "ERROR! Can only GetNextBases from a Pulse or Base File." << endl;
            assert(0);
            break;
    }
    if (stride > 1)
        Advance(stride-1);
    return numRecords;
}

int ReaderAgglomerate::GetNext(CCSSequence &seq) {
    int numRecords = 0;
    if (Subsample(subsample) == 0) {
        return 0;
    }

    switch(fileType) {
        case Fasta:
            // This just reads in the fasta sequence as if it were a ccs sequence
            numRecords = fastaReader.GetNext(seq);
            seq.subreadStart = 0;
            seq.subreadEnd   = 0;
            break;
        case Fastq:
            numRecords = fastqReader.GetNext(seq);
            seq.subreadStart = 0;
            seq.subreadEnd   = 0;
            break;
        case HDFPulse:
        case HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case HDFCCSONLY:
        case HDFCCS:
            numRecords = hdfCcsReader.GetNext(seq);
            break;
    }

    if (stride > 1)
        Advance(stride-1);
    return numRecords;
}


int ReaderAgglomerate::Advance(int nSteps) {
    int i;
    switch(fileType) {
        case Fasta:
            return fastaReader.Advance(nSteps);
        case HDFPulse:
        case HDFBase:
            return hdfBasReader.Advance(nSteps);
        case HDFCCSONLY:
        case HDFCCS:
            return hdfCcsReader.Advance(nSteps);
        case Fastq:
            return fastqReader.Advance(nSteps);
    }
    return false;
}

void ReaderAgglomerate::Close() {
    switch(fileType) {
        case Fasta:
            fastaReader.Close();
            break;
        case HDFPulse:
        case HDFBase:
            hdfBasReader.Close();
            // zmwReader.Close();
            break;
        case HDFCCS:
            hdfCcsReader.Close();
            break;
    }
}

