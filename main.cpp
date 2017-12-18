#include <iostream>
#include <cstdint>
#include <ctime>



uint32_t div10(uint32_t num, uint32_t &r)
{
	uint32_t q;

	q = num / 10;
	r = num - q * 10;

	return q;
}


uint32_t div10mag(uint32_t num, uint32_t &r)
{
	uint32_t q;

	q = (0x66666667ULL * num) >> 34;

	r = num - q * 10;

	return q;
}


uint32_t div10asmmag(uint32_t num, uint32_t &r)
{
	uint32_t q;
	
	__asm {
	mov eax, dword ptr[num];
	mov ecx, 66666667h;

	mul ecx;
	shr edx, 2;
	
	imul eax, edx, 0Ah;
	mov ecx, dword ptr[num];
	sub ecx, eax;

	mov dword ptr[q], edx;

	mov edx, dword ptr[r];
	mov dword ptr[edx], ecx;
	}

	return q;
}


uint32_t div10mod(uint32_t num, uint32_t &r)
{
	uint32_t q = 0;

	r = num % 10;

	return q;
}

int main()
{
	uint32_t r = 0;
	volatile uint32_t q;
	clock_t start, end;

	start = clock();
	for (uint32_t i = 0; i < UINT32_MAX; i++)
	{
		q = div10asmmag(i, r);
	}
	end = clock();

	std::cout << "asm magic" << std::endl;
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

	std::cout << std::endl << std::endl;
	start = clock();
	for (uint32_t i = 0; i < UINT32_MAX; i++)
	{
		q = div10mag(i, r);
	}
	end = clock();

	std::cout << "magic" << std::endl;
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

	std::cout << std::endl << std::endl;
	start = clock();
	for (uint32_t i = 0; i < UINT32_MAX; i++)
	{
		q = div10(i, r);
	}
	end = clock();

	std::cout << "div" << std::endl;
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

	std::cout << std::endl << std::endl;
	start = clock();
	for (uint32_t i = 0; i < UINT32_MAX; i++)
	{
		q = div10mod(i, r);
	}
	end = clock();

	std::cout << "mod" << std::endl;
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

	return 0;
}
