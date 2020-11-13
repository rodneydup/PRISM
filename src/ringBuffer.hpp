// #include <cmath>
#include <mutex>

class RingBuffer {
 public:
  RingBuffer(unsigned maxSize) : mMaxSize(maxSize) {
    mBuffer.resize(mMaxSize);
    mTail = -1;
    mPrevSample = 0;
  }

  unsigned getMaxSize() const { return mMaxSize; }

  void resize(unsigned maxSize) {
    mMaxSize = maxSize;
    mBuffer.resize(mMaxSize);
  }

  void push_back(float value) {
    mMutex.lock();
    mTail = (mTail + 1) % mMaxSize;
    mBuffer[mTail] = value;
    mMutex.unlock();
  }

  unsigned getTail() const { return mTail; }

  float at(unsigned index) {
    if (index >= mMaxSize) {
      std::cerr << "RingBuffer index out of range." << std::endl;
      index = index % mMaxSize;
    }
    if (mMutex.try_lock()) {
      mPrevSample = mBuffer.at(index);
      mMutex.unlock();
    }
    return mPrevSample;
  }

  float operator[](unsigned index) { return this->at(index); }

  const float *data() { return mBuffer.data(); }

  float getRMS(unsigned lookBackLength) {
    int start = mTail - lookBackLength;
    if (start < 0) start = mMaxSize + start;

    float val = 0.0;
    for (unsigned i = 0; i < lookBackLength; i++) {
      val += pow(mBuffer[(start + i) % mMaxSize], 2);
    }
    return sqrt(val / lookBackLength);
  }

  void print() const {
    for (auto i = mBuffer.begin(); i != mBuffer.end(); ++i) std::cout << *i << " ";
    std::cout << "\n";
  }

 private:
  std::vector<float> mBuffer;
  unsigned mMaxSize;
  int mTail;
  float mPrevSample;

  std::mutex mMutex;
};

struct Plot_RingBufferGetterData {
  const float *Values;
  int Stride;
  int RingOffset;
  int MaxRingSize;

  Plot_RingBufferGetterData(const float *values, int stride, int ring_offset, int max_ring_size) {
    Values = values;
    Stride = stride;
    RingOffset = ring_offset;
    MaxRingSize = max_ring_size;
  }
};