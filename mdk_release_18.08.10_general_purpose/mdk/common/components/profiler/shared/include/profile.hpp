#include <type_traits>
#include <cstring>
#include <memory>

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE (4000u)
#endif
#define unlikely(x) __builtin_expect(!!(x), 0)
// TODO: use serialization when available c++20 maybe. or cereal - http://uscilab.github.io/cereal/
class ProfileBuffer {
protected:
	template <typename T>
	inline __attribute__((always_inline)) void push(const T t) {
		static_assert(std::is_trivially_copyable<T>(), "You should push only copiable data");
		*reinterpret_cast<T*>(head) = t; // write into the buffer
		head += sizeof(T);
		if ( unlikely(head == end) ) {
			head = buffer;
		}
	}

	// TODO: perfimprovement - if text is constant, save only address
	inline __attribute__((always_inline)) void push(const char* text) {
		while(*text) {
			push(*text++);
		}
	}

	template <typename T, typename... Rest>
	inline __attribute__((always_inline)) void push(const T t, const Rest... args) {
		push(t);
		push(args...);
	}

public:
	ProfileBuffer(char* mem, size_t size)
	: buffer(mem)
	, end(mem+size)
	{
	//	Profiler::registerBuffer(this);
	}

	template <typename T, std::size_t N>
	ProfileBuffer(T(&m)[N])
	: buffer(m)
	, end(m+N)
	{
	//	Profiler::registerBuffer(this);
	}

	template <typename... Args>
	void pushEntry(const Args... args) {
		push(args...);
	}

private:
	char* const buffer;
	char* const end;
	char* __restrict__ head = buffer;
};

class VariableSizeBlockProfileBuffer: public ProfileBuffer
{
public:
	// VariableSizeBlockProfileBuffer(auto x, auto s) : ProfileBuffer(x, s) {
	// }
	template <typename... Args>
	void pushEntry(const Args... args) {
		push((uint16_t)size(args...), args...);
	}

private:
	static constexpr std::size_t size() { return 0; }
	template <typename H, typename... T>
	static constexpr std::size_t size(const H& h, const T&... t) {
		return sizeof(h) + size(t...);
	}
};

#if 0
int main() {
char buffer[5000];
ProfileBufferEntry_t e(buffer, sizeof(buffer));
	std::cout << "buffer addr = " << std::hex << std::addressof(buffer) << std::endl;
	e.pushEntry(9, 13, "abc", 'x');
	for(int i=0; i<20; i++) {
		std::cout << std::hex << (int)buffer[i] << " - " << buffer[i] << std::endl;
	}
	return 0;
}
#endif
