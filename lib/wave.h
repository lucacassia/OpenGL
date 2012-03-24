#ifndef WAVE
#define WAVE

#include<fstream>
#include<iostream>

class RIFF_Header{
    public:
        char chunkID[5];
        int chunkSize;
        char format[5];

        void setRiffHeader(std::ifstream& file){
            file.read(chunkID, 4);
            chunkID[4]='\0';
            file.read((char*)&chunkSize, 4);
            file.read(format, 4);
            format[4]='\0';
        }
};

class FMT_Chunk{
    public:
        char chunkID[5];
        int chunkSize;
        short audioFormat;
        short numChannels;
        int sampleRate;
        int byteRate;
        short blockAlign;
        short bitsPerSample;

        void setFmtChunk(std::ifstream& file){
            file.read(chunkID, 4);
            chunkID[4]='\0';
            file.read((char*)&chunkSize, 4);
            file.read((char*)&audioFormat, 2);
            file.read((char*)&numChannels, 2);
            file.read((char*)&sampleRate, 4);
            file.read((char*)&byteRate, 4);
            file.read((char*)&blockAlign, 2);
            file.read((char*)&bitsPerSample, 2);
        }
};

class DATA_Chunk{
    public:
        char chunkID[5];
        int chunkSize;

        void setDataChunk(std::ifstream& file){
            file.read(chunkID, 4);
            chunkID[4]='\0';
            file.read((char*)&chunkSize, 4);
        }
};

class wave{
    public:
        RIFF_Header riffHeader;
        FMT_Chunk fmtChunk;
        DATA_Chunk dataChunk;

        wave(const char* path_to_file){
            std::ifstream wave ( path_to_file, std::ios::in | std::ios::binary);
            riffHeader.setRiffHeader(wave);
            fmtChunk.setFmtChunk(wave);
            dataChunk.setDataChunk(wave);
        }

        void printInfo(){
            std::cout << "\nRIFF header:\n";

            printf (" ChunkID = %s \n", riffHeader.chunkID);
            std::cout << " ChunkSize = " << riffHeader.chunkSize << std::endl;
            printf (" Format = %s \n", riffHeader.format);

            std::cout << "\nfmt chunk:\n";

            printf (" ChunkID = %s \n", fmtChunk.chunkID);
            std::cout << " ChunkSize = " << fmtChunk.chunkSize << std::endl;
            std::cout << " audioFormat = " << fmtChunk.audioFormat << std::endl;
            std::cout << " numChannels = " << fmtChunk.numChannels << std::endl;
            std::cout << " sampleRate = " << fmtChunk.sampleRate << std::endl;
            std::cout << " byteRate = " << fmtChunk.byteRate << std::endl;
            std::cout << " blockAlign = " << fmtChunk.blockAlign << std::endl;
            std::cout << " bitsPerSample = " << fmtChunk.bitsPerSample << std::endl;

            std::cout << "\ndata chunk:\n";

            printf (" ChunkID = %s \n", dataChunk.chunkID);
            std::cout << " ChunkSize = " << dataChunk.chunkSize << std::endl;

            std::cout<<std::endl;

        }

};

#endif
