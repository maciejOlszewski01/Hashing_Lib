#include <iostream>
#include <bitset>


/*The full description for SHA224 function is provided, so after it for every other function ( if there is no necessity ) 
 only specified differences will be described, because there is no point in writing the same comments all over and over 
 again*/


namespace hash_lib
{
	 
	 
	enum decimal_to_hex_system																//converting decimal number 
	{																			//to 32-bit hexadecimal or
		x86 = 1,																	//to 64-bit hexadecimal
		x64 = 2
	};



	enum output_type																	//output convention
	{																			//can be either "little endian"
		LITTLE_ENDIAN = 1,																//or "big endian"
		BIG_ENDIAN = 2
	};
	
	
	


	//The body of every declared function below is at the end of file
					
	//The beginnig of function declaration section
	
	std::string decimal_to_hex(unsigned long long int, 															
							   hash_lib::decimal_to_hex_system = hash_lib::decimal_to_hex_system::x86, 				//default parameter
							   hash_lib::output_type = hash_lib::output_type::BIG_ENDIAN						//default parameter
							  );


	template<size_t T>																							
	inline std::bitset<T> rightrotate(std::bitset<T>, unsigned);												
																												
																												
																												
	template<size_t T>																							
	inline std::bitset<T> leftrotate(std::bitset<T>, unsigned);		

	
	//The end of function declaration section
	



	//SHA224
	//Registers: 32-bit
	//Block-size: 512-bit
	//Padding-size: 64-bit
	//Number of rounds: 64
	//Output convention: "Big Endian"
	//Constants used: "k"[64], "w"[64], "h"[8]
	//Input size: any
	//Number of "h" used in final output: 7
	//Output size: 224-bit
						
	std::string SHA224(std::string to_encrypt)
	{

		std::bitset<32> k[64]={0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,			 							//declaration and initialization of
							   0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 							//table "k" with 64 predefined
							   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 							//constants which are used later
							   0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,							//in rounds operations
							   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
							   0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
							   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
							   0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
							   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
							   0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
							   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
							   0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
							   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
							   0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
							   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
							   0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

		unsigned long long int length_of_message = (to_encrypt.size()*8);										//length of message in bits;
																				//multipled by 8, because
																				//every char is one byte

		length_of_message++;																//incrementation, because
																				//of "1" which we must always
																				//append to message 
		
		
		while(length_of_message%512 != 448)														//incrementing until length
			length_of_message++;															//of message is "mod 512 == 448",
																				//because there must be left place
																				//for last 64bit padding which will
																				//be added later

		std::bitset<8> temp_table[length_of_message/8];													//temporary table of 8-bit bitsets
																				//in which we will store message 
																				//bytes without place for 
																				//64bit padding yet

		for(int i = 0; i < to_encrypt.size(); i++)													//converting message chars from
			temp_table[i] = (int)to_encrypt[i];													//ASCII to 8-bit bitsets


		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");											//appending of mentioned earlier "1";
																				//after this "[to_encrypt.size()]"
																				//index every bitset in table
																				//is implicitly set to "0000 0000"

		std::bitset<64> padding_mess_len(to_encrypt.size()*8);												//creating 64-bit bitset for last
																				//padding and initializing with
																				//length of given string in bits
		
		std::string padding_mess_len_str = padding_mess_len.to_string();										//converting 64-bit padding
																				//bitset to string

		length_of_message += 64;															//adding 64-bit padding to lentgh
		
		int block_number = length_of_message/512;													//calculation of number of 512-bit
																				//blocks
				
		length_of_message /= 8;																//converting length in bits to bytes

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];											//creating table of 8-bit bitsets
																				//in which we will store full content
																				//of calculated bitsets until now

		for(int i = 0; i < length_of_message-8; i++)													//assigning table of 8-bit bitsets
			tab[i] = temp_table[i];															//without 64-bit padding

		for(int i = 0 ; i < 8; i++)															//assigning table of 8-bit bitsets
			tab[length_of_message-8 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));							//which in total gives us 64-bit
																				//padding
																												
		std::bitset<32> words_table[block_number][16];													//creating table of 32-bit bitsets
																				//which we will call "words"
		
		int g{0}, licz{0};																//auxiliary variables for loop
																				//beneath
		
		for(int i = 0; i < length_of_message; i += 4)													//converting 8-bit bitsets
		{																		//to 32-bit words
			if(licz == 16)																						
			{
				g++;
				licz = 0;
			}
			words_table[g][licz] = std::bitset<32>(tab[i].to_string() + tab[i+1].to_string() 
									+ tab[i+2].to_string() + tab[i+3].to_string());
			
			licz++;
		}

		delete [] tab;																	//freeing memory

		std::bitset<32> h1[block_number+1]={0xc1059ed8};												//eight constants "h" used later
		std::bitset<32> h2[block_number+1]={0x367cd507};												//in initialization of temporary
		std::bitset<32> h3[block_number+1]={0x3070dd17};												//(let's say) "registers" 
		std::bitset<32> h4[block_number+1]={0xf70e5939};												//in main part; also used for
		std::bitset<32> h5[block_number+1]={0xffc00b31};												//creating final output
		std::bitset<32> h6[block_number+1]={0x68581511};
		std::bitset<32> h7[block_number+1]={0x64f98fa7};
		std::bitset<32> h8[block_number+1]={0xbefa4fa4};
	

		for(int i = 1; i <= block_number; i++)														//start of computing blocks
		{
			std::bitset<32> reg_a = h1[i-1];													//temporary registers mentioned 
			std::bitset<32> reg_b = h2[i-1];													//earlier, initializied with
			std::bitset<32> reg_c = h3[i-1];													//"h" constants in "a = h1", 
			std::bitset<32> reg_d = h4[i-1];													//"b = h2", "c = h3" and etc. order
			std::bitset<32> reg_e = h5[i-1];
			std::bitset<32> reg_f = h6[i-1];
			std::bitset<32> reg_g = h7[i-1];
			std::bitset<32> reg_h = h8[i-1];


			std::bitset<32> T1, T2;															//declaration of two temporary
																				//bitsets used in main part
																				//for storing intermediate
																				//values between operations
			
			std::bitset<32> w[64];															//declaration of table "w"
																				//in which we will store constant
																				//values computed later
			
			std::bitset<32> s0, s1, ch, maj;													//declaration of four bitsets with
																				//names "ch" and "maj" resembling
																				//original names of functions used
																				//in main part (look at https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&cad=rja&uact=8&ved=2ahUKEwjCmvuWv7PiAhXSAxAIHcUdDckQFjACegQIARAC&url=http%3A%2F%2Fwww.iwar.org.uk%2Fcomsec%2Fresources%2Fcipher%2Fsha256-384-512.pdf&usg=AOvVaw2ci9yeB4IsouQyaS6esNJY);
																				//when computing "w" constants
																				//"s0" and "s1" acts as "o0" and "o1"
																				//and in main part as "E0" and "E1"
			
			for(int j = 0; j < 16; j++)														//assigning block words to 
				w[j] = words_table[i-1][j];													//"w" constants
		
			for(int j = 16; j < 64; j++)														//computing rest of "w" constants
			{	
				s0 = (hash_lib::rightrotate(w[j-15],7)^hash_lib::rightrotate(w[j-15], 18)^(w[j-15]>>3));
				
				s1 = (hash_lib::rightrotate(w[j-2],17)^hash_lib::rightrotate(w[j-2],19)^(w[j-2]>>10));
				
				w[j] = std::bitset<32>(w[j-16].to_ulong() + s0.to_ulong() + w[j-7].to_ulong() + s1.to_ulong());
			}	
						
			for(int j = 0; j < 64; j++)														//START OF MAIN PART
			{																									
																				//64 rounds in which set
				s1 = std::bitset<32>(hash_lib::rightrotate(reg_e,6)^hash_lib::rightrotate(reg_e,11)						//of function is used to compute 
									^hash_lib::rightrotate(reg_e,25));							//hash (again for more details 
																				//look at original reference
				ch = std::bitset<32>((reg_e&reg_f)^((~reg_e)&reg_g));										//mentioned as link above)
				
				T1 = std::bitset<32>(reg_h.to_ulong() + s1.to_ulong() + ch.to_ulong() 
									+ k[j].to_ulong() + w[j].to_ulong());
				
				s0 = std::bitset<32>(hash_lib::rightrotate(reg_a,2)^hash_lib::rightrotate(reg_a,13)
									^hash_lib::rightrotate(reg_a,22));
				
				maj = std::bitset<32>((reg_a&reg_b)^(reg_a&reg_c)^(reg_b&reg_c));
				
				T2 = std::bitset<32>(s0.to_ulong() + maj.to_ulong());
				
				reg_h = reg_g;
				reg_g = reg_f;
				reg_f = reg_e;
				
				reg_e = std::bitset<32>(reg_d.to_ulong() + T1.to_ulong());
				
				reg_d = reg_c;
				reg_c = reg_b;
				reg_b = reg_a;
				
				reg_a = std::bitset<32>(T1.to_ulong() + T2.to_ulong());

			/*	std::cout<<"Round = "<<j<<"  "<<hash_lib::decimal_to_hex(reg_a.to_ulong())							//uncomment if you wanna see
								 <<"  "<<hash_lib::decimal_to_hex(reg_b.to_ulong())						//rounds in action
								 <<"  "<<hash_lib::decimal_to_hex(reg_c.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_d.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_e.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_f.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_g.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_h.to_ulong())
				<<std::endl<<std::endl; */
			}																	//END OF MAIN PART

			h1[i] = std::bitset<32>(reg_a.to_ulong()+ h1[i-1].to_ulong());										//addition of output of temporary
			h2[i] = std::bitset<32>(reg_b.to_ulong() +h2[i-1].to_ulong());										//registers used in current block to 
			h3[i] = std::bitset<32>(reg_c.to_ulong()+ h3[i-1].to_ulong());										//"h" constants
			h4[i] = std::bitset<32>(reg_d.to_ulong() +h4[i-1].to_ulong());
			h5[i] = std::bitset<32>(reg_e.to_ulong() +h5[i-1].to_ulong());
			h6[i] = std::bitset<32>(reg_f.to_ulong() +h6[i-1].to_ulong());
			h7[i] = std::bitset<32>(reg_g.to_ulong() +h7[i-1].to_ulong());
			h8[i] = std::bitset<32>(reg_h.to_ulong() +h8[i-1].to_ulong());
				
		}

		std::string str_ret = hash_lib::decimal_to_hex( h1[block_number].to_ulong()) 									//creating final hash from
							+ hash_lib::decimal_to_hex( h2[block_number].to_ulong()) 						//seven "h" constants after 
							+ hash_lib::decimal_to_hex( h3[block_number].to_ulong()) 						//the computing of last block 
							+ hash_lib::decimal_to_hex( h4[block_number].to_ulong()) 							
							+ hash_lib::decimal_to_hex( h5[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h6[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h7[block_number].to_ulong()); 
																						
		return str_ret;																	//returning hash

	}
	
	
	//SHA256
	//Registers: 32-bit
	//Block-size: 512-bit
	//Padding-size: 64-bit
	//Number of rounds: 64
	//Output convention: "Big Endian"
	//Constants used: "k"[64], "w"[64], "h"[8]
	//Input size: any
	//Number of "h" used in final output: 8
	//Output size: 256-bit
	
	/*No point in commeting SHA256 hash computing, cause reasoning is almost the same like in
	  SHA224; only difference is initialization of "h" constants (different values than in SHA224);
	  of course length of output hash is longer by 32 bits  */
	
														
	std::string SHA256(std::string to_encrypt)
	{

		std::bitset<32> k[64]={0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
							   0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
							   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
							   0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
							   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
							   0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
							   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
							   0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
							   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
							   0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
							   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
							   0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
							   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
							   0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
							   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
							   0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);

		length_of_message++;

		while(length_of_message%512 != 448)
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<64> padding_mess_len(to_encrypt.size()*8);		

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 64;
		int block_number = length_of_message/512;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-8; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 8; i++)
			tab[length_of_message-8 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<32> words_table[block_number][16];
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 4)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}
			words_table[g][licz] = std::bitset<32>(tab[i].to_string() + tab[i+1].to_string() 
									+ tab[i+2].to_string() + tab[i+3].to_string());
			
			licz++;
		}

		delete [] tab;	
																				//DIFFERENCE
		std::bitset<32> h1[block_number+1]={0x6a09e667};												//initialization with different
		std::bitset<32> h2[block_number+1]={0xbb67ae85};												//values than in SHA224
		std::bitset<32> h3[block_number+1]={0x3c6ef372};
		std::bitset<32> h4[block_number+1]={0xa54ff53a};
		std::bitset<32> h5[block_number+1]={0x510e527f};
		std::bitset<32> h6[block_number+1]={0x9b05688c};
		std::bitset<32> h7[block_number+1]={0x1f83d9ab};
		std::bitset<32> h8[block_number+1]={0x5be0cd19};
	

		for(int i = 1; i <= block_number; i++)
		{
			std::bitset<32> reg_a = h1[i-1];
			std::bitset<32> reg_b = h2[i-1];
			std::bitset<32> reg_c = h3[i-1];
			std::bitset<32> reg_d = h4[i-1];
			std::bitset<32> reg_e = h5[i-1];
			std::bitset<32> reg_f = h6[i-1];
			std::bitset<32> reg_g = h7[i-1];
			std::bitset<32> reg_h = h8[i-1];


			std::bitset<32> T1, T2;

			std::bitset<32> w[64];

			std::bitset<32> s0, s1, ch, maj;

			for(int j = 0; j < 16; j++)
				w[j] = words_table[i-1][j];
		
			for(int j = 16; j < 64; j++)
			{	
				s0 = (hash_lib::rightrotate(w[j-15],7)^hash_lib::rightrotate(w[j-15], 18)^(w[j-15]>>3));
				
				s1 = (hash_lib::rightrotate(w[j-2],17)^hash_lib::rightrotate(w[j-2],19)^(w[j-2]>>10));
				
				w[j] = std::bitset<32>(w[j-16].to_ulong() + s0.to_ulong() + w[j-7].to_ulong() + s1.to_ulong());
			}	
						
			for(int j = 0; j < 64; j++)
			{
				s1 = std::bitset<32>(hash_lib::rightrotate(reg_e,6)^hash_lib::rightrotate(reg_e,11)
									^hash_lib::rightrotate(reg_e,25));
				
				ch = std::bitset<32>((reg_e&reg_f)^((~reg_e)&reg_g));
				
				T1 = std::bitset<32>(reg_h.to_ulong() + s1.to_ulong() + ch.to_ulong() 
									+ k[j].to_ulong() + w[j].to_ulong());
				
				s0 = std::bitset<32>(hash_lib::rightrotate(reg_a,2)^hash_lib::rightrotate(reg_a,13)
									^hash_lib::rightrotate(reg_a,22));
				
				maj = std::bitset<32>((reg_a&reg_b)^(reg_a&reg_c)^(reg_b&reg_c));
				
				T2 = std::bitset<32>(s0.to_ulong() + maj.to_ulong());
				
				reg_h = reg_g;
				reg_g = reg_f;
				reg_f = reg_e;
				
				reg_e = std::bitset<32>(reg_d.to_ulong() + T1.to_ulong());
				
				reg_d = reg_c;
				reg_c = reg_b;
				reg_b = reg_a;
				
				reg_a = std::bitset<32>(T1.to_ulong() + T2.to_ulong());
																												
			/*	std::cout<<"Round = "<<j<<"  "<<hash_lib::decimal_to_hex(reg_a.to_ulong())								//uncomment if you wanna see
								 <<"  "<<hash_lib::decimal_to_hex(reg_b.to_ulong())							//rounds in action
								 <<"  "<<hash_lib::decimal_to_hex(reg_c.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_d.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_e.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_f.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_g.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_h.to_ulong())
				<<std::endl<<std::endl; */
			}

			h1[i] = std::bitset<32>(reg_a.to_ulong()+ h1[i-1].to_ulong());
			h2[i] = std::bitset<32>(reg_b.to_ulong() +h2[i-1].to_ulong());
			h3[i] = std::bitset<32>(reg_c.to_ulong()+ h3[i-1].to_ulong());
			h4[i] = std::bitset<32>(reg_d.to_ulong() +h4[i-1].to_ulong());
			h5[i] = std::bitset<32>(reg_e.to_ulong() +h5[i-1].to_ulong());
			h6[i] = std::bitset<32>(reg_f.to_ulong() +h6[i-1].to_ulong());
			h7[i] = std::bitset<32>(reg_g.to_ulong() +h7[i-1].to_ulong());
			h8[i] = std::bitset<32>(reg_h.to_ulong() +h8[i-1].to_ulong());
				
		}
																					//DIFFERENCE
		std::string str_ret = hash_lib::decimal_to_hex( h1[block_number].to_ulong()) 										//creating final hash from
							+ hash_lib::decimal_to_hex( h2[block_number].to_ulong()) 							//eight "h" constants,not seven
							+ hash_lib::decimal_to_hex( h3[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h4[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h5[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h6[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h7[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h8[block_number].to_ulong());

		return str_ret;	

	}
	


	//SHA512
	//Registers: 64-bit
	//Block-size: 1024-bit
	//Padding-size: 128-bit
	//Number of rounds: 80
	//Output convention: "Big Endian"
	//Constants used: "k"[80], "w"[80], "h"[8]
	//Input size: any
	//Number of "h" used in final output: 8
	//Output size: 512-bit
	
	/*The difference between SHA256 and SHA512 is all about switching from 32-bit to 64-bit system, which
	  gives us in return enlarged registers from  32-bit to 64-bit, block-size from 512-bit to 1024-bit and
	  padding size from 64-bit to 128-bit. Also number of constants "w", "k" and rounds increases (from 64 to 80).
	  */

	std::string SHA512(std::string to_encrypt)
	{

		std::bitset<64> k[80]={0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 							//DIFFERENCE
							   0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 				//80 "k" constants instead of 64
							   0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 				//also they're 64-bit, not 32-bit
							   0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, 
							   0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
							   0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 
							   0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 
							   0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70, 
							   0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, 
							   0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
							   0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 
							   0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 
							   0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 
							   0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 
							   0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
							   0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 
							   0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 
							   0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b, 
							   0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
							   0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);

		length_of_message++;

		while(length_of_message%1024 != 896)															//1024-bit blocks instead of 512-bit
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<128> padding_mess_len(to_encrypt.size()*8);													//padding is 128-bit, not 64-bit

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 128;
		int block_number = length_of_message/1024;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-16; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 16; i++)
			tab[length_of_message-16 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<64> words_table[block_number][16];														//words in blocks are 64-bit, not 32-bit
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 8)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}

			words_table[g][licz] = std::bitset<64>(tab[i].to_string() + tab[i+1].to_string() 
												+ tab[i+2].to_string() + tab[i+3].to_string() 
												+ tab[i+4].to_string() + tab[i+5].to_string() 
												+ tab[i+6].to_string() + tab[i+7].to_string()
												);
			licz++;
		}

		delete [] tab;

		std::bitset<64> h1[block_number+1]={0x6a09e667f3bcc908};												//"h" constants are 64-bit, not 32-bit
		std::bitset<64> h2[block_number+1]={0xbb67ae8584caa73b};
		std::bitset<64> h3[block_number+1]={0x3c6ef372fe94f82b};
		std::bitset<64> h4[block_number+1]={0xa54ff53a5f1d36f1};
		std::bitset<64> h5[block_number+1]={0x510e527fade682d1};
		std::bitset<64> h6[block_number+1]={0x9b05688c2b3e6c1f};
		std::bitset<64> h7[block_number+1]={0x1f83d9abfb41bd6b};
		std::bitset<64> h8[block_number+1]={0x5be0cd19137e2179};

		for(int i = 1; i <= block_number; i++)
		{	
			std::bitset<64> reg_a = h1[i-1];
			std::bitset<64> reg_b = h2[i-1];
			std::bitset<64> reg_c = h3[i-1];
			std::bitset<64> reg_d = h4[i-1];
			std::bitset<64> reg_e = h5[i-1];
			std::bitset<64> reg_f = h6[i-1];
			std::bitset<64> reg_g = h7[i-1];
			std::bitset<64> reg_h = h8[i-1];

			std::bitset<64> T1, T2;

			std::bitset<64> w[80];

			std::bitset<64> s0, s1, ch, maj;

			for(int j = 0; j < 16; j++)
			{
				w[j] = words_table[i-1][j];
			}
				
			for(int j = 16; j < 80; j++)															//as you see, more rounds in computing
			{																		//"w" constants; 64 -> 80
				s0 = (hash_lib::rightrotate(w[j-15],1)^hash_lib::rightrotate(w[j-15], 8)^(w[j-15]>>7));
				s1 = (hash_lib::rightrotate(w[j-2],19)^hash_lib::rightrotate(w[j-2],61)^(w[j-2]>>6));
				w[j] = std::bitset<64>(w[j-16].to_ullong() + s0.to_ullong() + w[j-7].to_ullong() + s1.to_ullong());
			}	
						
			for(int j = 0; j < 80; j++)															//also more rounds in main part
			{																		// 64 -> 80
				s1 = std::bitset<64>(hash_lib::rightrotate(reg_e,14)^hash_lib::rightrotate(reg_e,18)
									^hash_lib::rightrotate(reg_e,41));
				
				ch = std::bitset<64>((reg_e&reg_f)^((~reg_e)&reg_g));
				
				T1 = std::bitset<64>(reg_h.to_ullong() + s1.to_ullong() + ch.to_ullong()
									+ k[j].to_ullong() + w[j].to_ullong());
									
				s0 = std::bitset<64>(hash_lib::rightrotate(reg_a,28)^hash_lib::rightrotate(reg_a,34)
									^hash_lib::rightrotate(reg_a,39));
				
				maj = std::bitset<64>((reg_a&reg_b)^(reg_a&reg_c)^(reg_b&reg_c));
				
				T2 = std::bitset<64>(s0.to_ullong() + maj.to_ullong());
				
				reg_h = reg_g;
				reg_g = reg_f;
				reg_f = reg_e;
				
				reg_e = std::bitset<64>(reg_d.to_ullong() + T1.to_ullong());
				
				reg_d = reg_c;
				reg_c = reg_b;
				reg_b = reg_a;
				
				reg_a = std::bitset<64>(T1.to_ullong() + T2.to_ullong());

	/*			std::cout<<"Round = "<<j														//if you wanna see rounds in 
					<<"  "<<hash_lib::decimal_to_hex(reg_a.to_ullong(), hash_lib::decimal_to_hex_system::x64)					//action, uncomment
					<<"  "<<hash_lib::decimal_to_hex(reg_b.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_c.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_d.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_e.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_f.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_g.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_h.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<std::endl<<std::endl; */
			}

			h1[i] = std::bitset<64>(reg_a.to_ullong()+ h1[i-1].to_ullong());
			h2[i] = std::bitset<64>(reg_b.to_ullong() +h2[i-1].to_ullong());
			h3[i] = std::bitset<64>(reg_c.to_ullong()+ h3[i-1].to_ullong());
			h4[i] = std::bitset<64>(reg_d.to_ullong() +h4[i-1].to_ullong());
			h5[i] = std::bitset<64>(reg_e.to_ullong() +h5[i-1].to_ullong());
			h6[i] = std::bitset<64>(reg_f.to_ullong() +h6[i-1].to_ullong());
			h7[i] = std::bitset<64>(reg_g.to_ullong() +h7[i-1].to_ullong());
			h8[i] = std::bitset<64>(reg_h.to_ullong() +h8[i-1].to_ullong());
				
		}

		std::string str_ret = 																	//creating final hash output
			hash_lib::decimal_to_hex( h1[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 						//from eight "h" constants
			hash_lib::decimal_to_hex( h2[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h3[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h4[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h5[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h6[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h7[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h8[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64);

		return str_ret;	

	}
	

	//SHA384
	//Registers: 64-bit
	//Block-size: 1024-bit
	//Padding-size: 128-bit
	//Number of rounds: 80
	//Output convention: "Big Endian"
	//Constants used: "k"[80], "w"[80], "h"[8]
	//Input size: any
	//Number of "h" used in final output: 6
	//Output size: 384-bit
	
	/*No point in commeting SHA384 hash computing, cause reasoning is almost the same like in
	  SHA512; only difference is initialization of "h" constants (different values than in SHA512);
	  of course length of output hash is shorter by 128 bits  */

	std::string SHA384(std::string to_encrypt)
	{

		std::bitset<64> k[80]={0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 
							   0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 
							   0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 
							   0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, 
							   0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
							   0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 
							   0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 
							   0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70, 
							   0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df, 
							   0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
							   0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 
							   0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 
							   0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 
							   0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 
							   0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
							   0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 
							   0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 
							   0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b, 
							   0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 
							   0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);

		length_of_message++;

		while(length_of_message%1024 != 896)
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<128> padding_mess_len(to_encrypt.size()*8);		

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 128;
		int block_number = length_of_message/1024;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-16; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 16; i++)
			tab[length_of_message-16 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<64> words_table[block_number][16];
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 8)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}

			words_table[g][licz] = std::bitset<64>(tab[i].to_string() + tab[i+1].to_string() 
												  + tab[i+2].to_string() + tab[i+3].to_string() 
												  + tab[i+4].to_string() + tab[i+5].to_string() 
												  + tab[i+6].to_string() + tab[i+7].to_string()
												  );
			licz++;
		}

		delete [] tab;
															
		std::bitset<64> h1[block_number+1]={0xcbbb9d5dc1059ed8};												//DIFFERENCE
		std::bitset<64> h2[block_number+1]={0x629a292a367cd507};												//different "h" values initialization
		std::bitset<64> h3[block_number+1]={0x9159015a3070dd17};												//when compared to SHA512
		std::bitset<64> h4[block_number+1]={0x152fecd8f70e5939};
		std::bitset<64> h5[block_number+1]={0x67332667ffc00b31};
		std::bitset<64> h6[block_number+1]={0x8eb44a8768581511};
		std::bitset<64> h7[block_number+1]={0xdb0c2e0d64f98fa7};
		std::bitset<64> h8[block_number+1]={0x47b5481dbefa4fa4};

		for(int i = 1; i <= block_number; i++)
		{	
			std::bitset<64> reg_a = h1[i-1];
			std::bitset<64> reg_b = h2[i-1];
			std::bitset<64> reg_c = h3[i-1];
			std::bitset<64> reg_d = h4[i-1];
			std::bitset<64> reg_e = h5[i-1];
			std::bitset<64> reg_f = h6[i-1];
			std::bitset<64> reg_g = h7[i-1];
			std::bitset<64> reg_h = h8[i-1];


			std::bitset<64> T1, T2;

			std::bitset<64> w[80];

			std::bitset<64> s0, s1, ch, maj;

			for(int j = 0; j < 16; j++)
			{
				w[j] = words_table[i-1][j];
			}
				
			for(int j = 16; j < 80; j++)
			{	
				s0 = (hash_lib::rightrotate(w[j-15],1)^hash_lib::rightrotate(w[j-15], 8)^(w[j-15]>>7));
				s1 = (hash_lib::rightrotate(w[j-2],19)^hash_lib::rightrotate(w[j-2],61)^(w[j-2]>>6));
				w[j] = std::bitset<64>(w[j-16].to_ullong() + s0.to_ullong() + w[j-7].to_ullong() + s1.to_ullong());
			}	
						
			for(int j = 0; j < 80; j++)
			{
				s1 = std::bitset<64>(hash_lib::rightrotate(reg_e,14)^hash_lib::rightrotate(reg_e,18)
									^hash_lib::rightrotate(reg_e,41));
				
				ch = std::bitset<64>((reg_e&reg_f)^((~reg_e)&reg_g));
				
				T1 = std::bitset<64>(reg_h.to_ullong() + s1.to_ullong() + ch.to_ullong() 
									 + k[j].to_ullong() + w[j].to_ullong());
									 
				s0 = std::bitset<64>(hash_lib::rightrotate(reg_a,28)^hash_lib::rightrotate(reg_a,34)
									^hash_lib::rightrotate(reg_a,39));
				
				maj = std::bitset<64>((reg_a&reg_b)^(reg_a&reg_c)^(reg_b&reg_c));
				
				T2 = std::bitset<64>(s0.to_ullong() + maj.to_ullong());
				
				reg_h = reg_g;
				reg_g = reg_f;
				reg_f = reg_e;
				
				reg_e = std::bitset<64>(reg_d.to_ullong() + T1.to_ullong());
				
				reg_d = reg_c;
				reg_c = reg_b;
				reg_b = reg_a;
				
				reg_a = std::bitset<64>(T1.to_ullong() + T2.to_ullong());

			/*	std::cout<<"Round = "<<j														//if you wanna see rounds
					<<"  "<<hash_lib::decimal_to_hex(reg_a.to_ullong(), hash_lib::decimal_to_hex_system::x64)					//in action, uncomment
					<<"  "<<hash_lib::decimal_to_hex(reg_b.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_c.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_d.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_e.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_f.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_g.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<"  "<<hash_lib::decimal_to_hex(reg_h.to_ullong(), hash_lib::decimal_to_hex_system::x64)
					<<std::endl<<std::endl; */
			}

			h1[i] = std::bitset<64>(reg_a.to_ullong()+ h1[i-1].to_ullong());
			h2[i] = std::bitset<64>(reg_b.to_ullong() +h2[i-1].to_ullong());
			h3[i] = std::bitset<64>(reg_c.to_ullong()+ h3[i-1].to_ullong());
			h4[i] = std::bitset<64>(reg_d.to_ullong() +h4[i-1].to_ullong());
			h5[i] = std::bitset<64>(reg_e.to_ullong() +h5[i-1].to_ullong());
			h6[i] = std::bitset<64>(reg_f.to_ullong() +h6[i-1].to_ullong());
			h7[i] = std::bitset<64>(reg_g.to_ullong() +h7[i-1].to_ullong());
			h8[i] = std::bitset<64>(reg_h.to_ullong() +h8[i-1].to_ullong());
				
		}

		std::string str_ret = 																	//creating final hash output 			
			hash_lib::decimal_to_hex( h1[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 						//from six "h" constants
			hash_lib::decimal_to_hex( h2[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h3[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h4[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h5[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64) + 
			hash_lib::decimal_to_hex( h6[block_number].to_ullong(), hash_lib::decimal_to_hex_system::x64);

		return str_ret;	

	}
	


	//MD5
	//Registers: 32-bit
	//Block-size: 512-bit
	//Padding-size: 64-bit
	//Number of main-rounds: 4
	//Number of sub-rounds: 16
	//Output convention: "Little Endian"		!!!IMPORTANT!!!
	//Constants used: "k"[64], "s"[64]
	//Input size: any
	//Four constants used in final output: A, B, C, D
	//Output size: 128-bit

	std::string MD5(std::string to_encrypt)
	{

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);										//the first part of algorithm is
																					//the same like in SHA224
		length_of_message++;

		while(length_of_message%512 != 448)
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<64> padding_mess_len(to_encrypt.size()*8);		

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 64;
		int block_number = length_of_message/512;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-8; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 8; i++)
			tab[length_of_message-8 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<32> words_table[block_number][16];
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 4)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}
			
			
			words_table[g][licz] = std::bitset<32>(tab[i+3].to_string() + tab[i+2].to_string() 								//DIFFERENCE
												+ tab[i+1].to_string() + tab[i].to_string());				//words in blocks computed in 
																					//little endian convention
			licz++;
		}
		
		
		std::bitset<32> temporary_1  = std::bitset<32>( words_table[block_number-1][15].to_string().substr(24,8) +
											words_table[block_number-1][15].to_string().substr(16,8) +
											words_table[block_number-1][15].to_string().substr(8,8) +
											words_table[block_number-1][15].to_string().substr(0,8)); 
		
		
		std::bitset<32> temporary_2 = std::bitset<32>( words_table[block_number-1][14].to_string().substr(24,8) +
											words_table[block_number-1][14].to_string().substr(16,8) +
											words_table[block_number-1][14].to_string().substr(8,8) +
											words_table[block_number-1][14].to_string().substr(0,8));

		
		words_table[block_number-1][14] = temporary_1;														//EXCEPTION !!!!!!!!!!
		words_table[block_number-1][15] = temporary_2;														//two last words being padding
																					//are changed with each other, cause
																					//in this case little endian convention
																					//is used on whole words, not on bytes
		delete [] tab;																		//in words

		unsigned s[64]={7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,										//64 "s" constants
						5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
						4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
						6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

		std::bitset<32> k[64]={0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,											//64 "k" constants
							   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
							   0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
							   0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
							   0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
							   0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
							   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
							   0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
							   0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
							   0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
							   0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
							   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
							   0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
							   0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
							   0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
							   0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

		std::bitset<32> A=(0x67452301);																//four A, B, C, D constants initialization
		std::bitset<32> B=(0xefcdab89);
		std::bitset<32> C=(0x98badcfe);
		std::bitset<32> D=(0x10325476);	

		for(int i = 1; i <= block_number; i++)															
		{
			std::bitset<32> reg_a = A;
			std::bitset<32> reg_b = B;
			std::bitset<32> reg_c = C;
			std::bitset<32> reg_d = D;

			std::bitset<32> tempF;
			std::bitset<32> tempD;
			unsigned int g{0};
						
			for(int j = 0; j < 64; j++)															//MAIN PART
			{																		//constist of 4 main rounds divided into
				if(j <= 15)																//16 sub-rounds in which different operations
				{ 																	//take place
					tempF = std::bitset<32>((reg_b&reg_c)|((~reg_b)&reg_d));
					g = j;

				}else if(j <= 31 && j>=  16)
				{
					tempF = std::bitset<32>((reg_d&reg_b)|((~reg_d)&reg_c));
					g = (5*j + 1)%16;

				}else if(j <= 47 && j >= 32)
				{
					tempF = std::bitset<32>((reg_b^reg_c)^reg_d);
					g = (3*j + 5)%16;

				}else if(j <= 63 && j >= 48)
				{
					tempF = std::bitset<32>(reg_c^(reg_b|(~reg_d)));
					g = (7*j)%16;
				}
				
				tempD = reg_d;
				reg_d = reg_c;
				reg_c = reg_b;
				
				reg_b = std::bitset<32>(reg_b.to_ulong() + hash_lib::leftrotate(
						std::bitset<32>(reg_a.to_ulong() + tempF.to_ulong() + 
						k[j].to_ulong()+ words_table[i-1][g].to_ulong()), s[j]).to_ulong()
						);
						
				reg_a = tempD;		
			
			/*	std::cout<<"Round = "<<j<<" "<<hash_lib::decimal_to_hex(reg_a.to_ulong())								//uncomment, if you wanna see rounds
							  <<"  "<<hash_lib::decimal_to_hex(reg_b.to_ulong())								//in action
							  <<"  "<<hash_lib::decimal_to_hex(reg_c.to_ulong())
							  <<"  "<<hash_lib::decimal_to_hex(reg_d.to_ulong())
				<<"  "<<std::endl; */
			}

			A = std::bitset<32>(reg_a.to_ulong()+ A.to_ulong());
			B = std::bitset<32>(reg_b.to_ulong() +B.to_ulong());
			C = std::bitset<32>(reg_c.to_ulong()+ C.to_ulong());
			D = std::bitset<32>(reg_d.to_ulong() +D.to_ulong());
				
		}

		std::string D_ =	hash_lib::decimal_to_hex( D.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,							//computing final hash from A, B, C, D
										hash_lib::output_type::LITTLE_ENDIAN);							//remembering about little endian convention
		std::string C_ =	hash_lib::decimal_to_hex( C.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
		std::string B_ =	hash_lib::decimal_to_hex( B.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
		std::string A_ =	hash_lib::decimal_to_hex( A.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
										
		std::string str_ret = A_ + B_ + C_ + D_;
		return str_ret;																		//returning hash

	}
	
	
	//SHA1
	//Registers: 32-bit
	//Block-size: 512-bit
	//Padding-size: 64-bit
	//Number of main-rounds: 4
	//Number of sub-rounds: 20
	//Output convention: "Big Endian"		
	//Constants used: "w"[80], "h"[5]
	//Input size: any
	//Number of "h" used in final output: 5
	//Output size: 160-bit
	
	/*Reasoning like in MD5 algorithm, only differences are: 
	 - output convention: big endian
	 - output size: 160-bit, not 128-bit
	 - five constants instead of four ( we return to terminology "h constants" used earlier)
	 - number of sub-round enlarged from 16 to 20
	 - we get ride of "k" constants	*/
	
	std::string SHA1(std::string to_encrypt)
	{

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);

		length_of_message++;

		while(length_of_message%512 != 448)
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<64> padding_mess_len(to_encrypt.size()*8);		

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 64;
		int block_number = length_of_message/512;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-8; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 8; i++)
			tab[length_of_message-8 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<32> words_table[block_number][16];
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 4)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}
			words_table[g][licz] = std::bitset<32>(tab[i].to_string() + tab[i+1].to_string() 
									+ tab[i+2].to_string() + tab[i+3].to_string());
			
			licz++;
		}

		delete [] tab;	

		std::bitset<32> h1[block_number+1]={0x67452301};													//DIFFERENCE
		std::bitset<32> h2[block_number+1]={0xefcdab89};													//bigger number of constants
		std::bitset<32> h3[block_number+1]={0x98badcfe};													//five instead of four
		std::bitset<32> h4[block_number+1]={0x10325476};
		std::bitset<32> h5[block_number+1]={0xc3d2e1f0};
	

		for(int i = 1; i <= block_number; i++)
		{
			std::bitset<32> reg_a = h1[i-1];
			std::bitset<32> reg_b = h2[i-1];
			std::bitset<32> reg_c = h3[i-1];
			std::bitset<32> reg_d = h4[i-1];
			std::bitset<32> reg_e = h5[i-1];


			std::bitset<32> w[80];

			std::bitset<32> temp_F, temp_, temp_a;
			unsigned long int k;

			for(int j = 0; j < 16; j++)
				w[j] = words_table[i-1][j];
		
			for(int j = 16; j < 80; j++)
			{	
				std::bitset<32> temp_1 = std::bitset<32>(w[j-3]^w[j-8]^w[j-14]^w[j-16]);
				temp_1 = hash_lib::leftrotate(temp_1, 1);
				w[j] = temp_1;
			}	
			
						
			for(int j = 0; j < 80; j++)															//MAIN PART
			{																		//4 main rounds divided into
				if(j <= 19)																//20 sub-rounds
				{
					temp_F = ((reg_b & reg_c) | ((~reg_b) & reg_d));
					k = 0x5a827999;
					
				}else if( j >= 20 && j <= 39)
				{
					temp_F = (reg_b^reg_c^reg_d);
					k = 0x6ed9eba1;
					
				}else if(j >= 40 && j <= 59)
				{
					temp_F = ((reg_b & reg_c)|(reg_b & reg_d)|(reg_c & reg_d));
					k = 0x8f1bbcdc;
					
				}else if(j >= 60 && j <= 79)
				{
					temp_F = (reg_b^reg_c^reg_d);
					k = 0xca62c1d6;
				}
				
				temp_a = hash_lib::leftrotate(reg_a, 5);
				
				temp_ = std::bitset<32>(temp_a.to_ulong() + temp_F.to_ulong() + reg_e.to_ulong() + k + w[j].to_ulong());
				
				reg_e = reg_d;
				reg_d = reg_c;
				
				reg_c = hash_lib::leftrotate(reg_b, 30);
				
				reg_b = reg_a;
				reg_a = temp_;
				

			/*	std::cout<<"Round = "<<j<<"  "<<hash_lib::decimal_to_hex(reg_a.to_ulong())								//uncomment, if you wanna see rounds	
								 <<"  "<<hash_lib::decimal_to_hex(reg_b.to_ulong())							//in action
								 <<"  "<<hash_lib::decimal_to_hex(reg_c.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_d.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_e.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_f.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_g.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_h.to_ulong())
				<<std::endl<<std::endl; */
			}

			h1[i] = std::bitset<32>(reg_a.to_ulong()+ h1[i-1].to_ulong());
			h2[i] = std::bitset<32>(reg_b.to_ulong() +h2[i-1].to_ulong());
			h3[i] = std::bitset<32>(reg_c.to_ulong()+ h3[i-1].to_ulong());
			h4[i] = std::bitset<32>(reg_d.to_ulong() +h4[i-1].to_ulong());
			h5[i] = std::bitset<32>(reg_e.to_ulong() +h5[i-1].to_ulong());
			
				
		}
	

		std::string str_ret = hash_lib::decimal_to_hex( h1[block_number].to_ulong()) 										//computing final hash from
							+ hash_lib::decimal_to_hex( h2[block_number].to_ulong()) 							//5 "h" constants
							+ hash_lib::decimal_to_hex( h3[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h4[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h5[block_number].to_ulong()); 
		
		return str_ret;																		//returning hash

	}
	
	

	//SHA0
	//Registers: 32-bit
	//Block-size: 512-bit
	//Padding-size: 64-bit
	//Number of main-rounds: 4
	//Number of sub-rounds: 20
	//Output convention: "Big Endian"		
	//Constants used: "w"[80], "h"[5]
	//Input size: any
	//Number of "h" used in final output: 5
	//Output size: 160-bit
	
	/* Everything the same like in SHA1, only one difference - circular bitwise shift
	 doesn't occure while computing "w" constants	*/
	
	std::string SHA0(std::string to_encrypt)
	{

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);

		length_of_message++;

		while(length_of_message%512 != 448)
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<64> padding_mess_len(to_encrypt.size()*8);		

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 64;
		int block_number = length_of_message/512;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-8; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 8; i++)
			tab[length_of_message-8 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<32> words_table[block_number][16];
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 4)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}
			words_table[g][licz] = std::bitset<32>(tab[i].to_string() + tab[i+1].to_string() 
									+ tab[i+2].to_string() + tab[i+3].to_string());
			
			licz++;
		}

		delete [] tab;	

		std::bitset<32> h1[block_number+1]={0x67452301};
		std::bitset<32> h2[block_number+1]={0xefcdab89};
		std::bitset<32> h3[block_number+1]={0x98badcfe};
		std::bitset<32> h4[block_number+1]={0x10325476};
		std::bitset<32> h5[block_number+1]={0xc3d2e1f0};
	

		for(int i = 1; i <= block_number; i++)
		{
			std::bitset<32> reg_a = h1[i-1];
			std::bitset<32> reg_b = h2[i-1];
			std::bitset<32> reg_c = h3[i-1];
			std::bitset<32> reg_d = h4[i-1];
			std::bitset<32> reg_e = h5[i-1];

			std::bitset<32> w[80];

			std::bitset<32> temp_F, temp_, temp_a;
			unsigned long int k;

			for(int j = 0; j < 16; j++)
				w[j] = words_table[i-1][j];
		
			for(int j = 16; j < 80; j++)
			{	
				std::bitset<32> temp_1 = std::bitset<32>(w[j-3]^w[j-8]^w[j-14]^w[j-16]);								//DIFFERENCE
				w[j] = temp_1;																//circular bitwise shift doesn't occure
			}	
			
						
			for(int j = 0; j < 80; j++)
			{
				if(j <= 19)
				{
					temp_F = ((reg_b & reg_c) | ((~reg_b) & reg_d));
					k = 0x5a827999;
					
				}else if( j >= 20 && j <= 39)
				{
					temp_F = (reg_b^reg_c^reg_d);
					k = 0x6ed9eba1;
					
				}else if(j >= 40 && j <= 59)
				{
					temp_F = ((reg_b & reg_c)|(reg_b & reg_d)|(reg_c & reg_d));
					k = 0x8f1bbcdc;
					
				}else if(j >= 60 && j <= 79)
				{
					temp_F = (reg_b^reg_c^reg_d);
					k = 0xca62c1d6;
				}
				
				temp_a = hash_lib::leftrotate(reg_a, 5);
				
				temp_ = std::bitset<32>(temp_a.to_ulong() + temp_F.to_ulong() + reg_e.to_ulong() + k + w[j].to_ulong());
				
				reg_e = reg_d;
				reg_d = reg_c;
				
				reg_c = hash_lib::leftrotate(reg_b, 30);
				
				reg_b = reg_a;
				reg_a = temp_;
				

			/*	std::cout<<"Round = "<<j<<"  "<<hash_lib::decimal_to_hex(reg_a.to_ulong())								//uncomment, if you wanna see 
								 <<"  "<<hash_lib::decimal_to_hex(reg_b.to_ulong())							//rounds in action
								 <<"  "<<hash_lib::decimal_to_hex(reg_c.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_d.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_e.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_f.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_g.to_ulong())
								 <<"  "<<hash_lib::decimal_to_hex(reg_h.to_ulong())
				<<std::endl<<std::endl; */
			}

			h1[i] = std::bitset<32>(reg_a.to_ulong()+ h1[i-1].to_ulong());
			h2[i] = std::bitset<32>(reg_b.to_ulong() +h2[i-1].to_ulong());
			h3[i] = std::bitset<32>(reg_c.to_ulong()+ h3[i-1].to_ulong());
			h4[i] = std::bitset<32>(reg_d.to_ulong() +h4[i-1].to_ulong());
			h5[i] = std::bitset<32>(reg_e.to_ulong() +h5[i-1].to_ulong());
			
				
		}
	

		std::string str_ret = hash_lib::decimal_to_hex( h1[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h2[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h3[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h4[block_number].to_ulong()) 
							+ hash_lib::decimal_to_hex( h5[block_number].to_ulong()); 
		
		return str_ret;	

	}
	
	
	//MD4
	//Registers: 32-bit
	//Block-size: 512-bit
	//Padding-size: 64-bit
	//Number of main-rounds: 3
	//Number of sub-rounds: 16
	//Output convention: "Little Endian"		!!!IMPORTANT!!!
	//Constants used: "s"[48]
	//Input size: any
	//Four constants used in final output: A, B, C, D
	//Output size: 128-bit
	
	/*The same like MD5, difference occures in number of main rounds and
	 constants used - the number of them is smaller (64 -> 48) */
	
	std::string MD4(std::string to_encrypt)
	{

		unsigned long long int length_of_message = (to_encrypt.size()*8 %0xffffffff);

		length_of_message++;

		while(length_of_message%512 != 448)
			length_of_message++;
		

		std::bitset<8> temp_table[length_of_message/8];					

		for(int i = 0; i < to_encrypt.size(); i++)
			temp_table[i] = (int)to_encrypt[i];

		temp_table[to_encrypt.size()] = std::bitset<8>("10000000");

		std::bitset<64> padding_mess_len(to_encrypt.size()*8);		

		std::string padding_mess_len_str = padding_mess_len.to_string();

		length_of_message += 64;
		int block_number = length_of_message/512;
		length_of_message /= 8;

		std::bitset<8>* tab = new std::bitset<8>[length_of_message];

		for(int i = 0; i < length_of_message-8; i++)
			tab[i] = temp_table[i];

		for(int i = 0 ; i < 8; i++)
			tab[length_of_message-8 + i] = std::bitset<8>(padding_mess_len_str.substr(i * 8, 8));

		std::bitset<32> words_table[block_number][16];
		int g = 0;
		int licz = 0;
		for(int i = 0; i < length_of_message; i += 4)
		{
			if(licz == 16)
			{
				g++;
				licz = 0;
			}
			
			
			words_table[g][licz] = std::bitset<32>(tab[i+3].to_string() + tab[i+2].to_string() 
												+ tab[i+1].to_string() + tab[i].to_string());
												
			licz++;
		}
		
		std::bitset<32> bb = std::bitset<32>( words_table[block_number-1][15].to_string().substr(24,8) +
											words_table[block_number-1][15].to_string().substr(16,8) +
											words_table[block_number-1][15].to_string().substr(8,8) +
											words_table[block_number-1][15].to_string().substr(0,8)); 
		
		std::bitset<32> aa = std::bitset<32>( words_table[block_number-1][14].to_string().substr(24,8) +
											words_table[block_number-1][14].to_string().substr(16,8) +
											words_table[block_number-1][14].to_string().substr(8,8) +
											words_table[block_number-1][14].to_string().substr(0,8));

		
		words_table[block_number-1][14] = bb;
		words_table[block_number-1][15] = aa;
		

		delete [] tab;

		unsigned s[48]={3, 7, 11, 19,  3, 7, 11, 19,  3, 7, 11, 19,  3, 7, 11, 19,										//DIFFERENCE
						3,  5, 9, 13,  3,  5, 9, 13,  3,  5, 9, 13,  3,  5, 9, 13,								//number of "s" constants is 48
						3, 9, 11, 15,  3, 9, 11, 15,  3, 9, 11, 15,  3, 9, 11, 15,};								//instead of 64
						
		unsigned p[48]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
						0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15,
						0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

		unsigned long k;

		std::bitset<32> A=(0x67452301);
		std::bitset<32> B=(0xefcdab89);
		std::bitset<32> C=(0x98badcfe);
		std::bitset<32> D=(0x10325476);	

		for(int i = 1; i <= block_number; i++)
		{
			std::bitset<32> reg_a = A;
			std::bitset<32> reg_b = B;
			std::bitset<32> reg_c = C;
			std::bitset<32> reg_d = D;

			std::bitset<32> tempF, tempD;
						
			for(int j = 0; j < 48; j++)															//MAIN PART
			{																		//the number of rounds is smaller
				if(j <= 15)																//48 instead of 64
				{
					tempF = std::bitset<32>((reg_b&reg_c)|((~reg_b)&reg_d));
					k = 0;

				}else if(j <= 31 && j>=  16)
				{
					tempF = std::bitset<32>((reg_b&reg_c)|(reg_b&reg_d)|(reg_c&reg_d));
					k = 0x5a827999;

				}else if(j <= 47 && j >= 32)
				{
					tempF = std::bitset<32>((reg_b^reg_c)^reg_d);
					k = 0x6ed9eba1;
				}		
				
				tempD = reg_d;
				reg_d = reg_c;
				reg_c = reg_b;
				
				reg_b = std::bitset<32>( 
							hash_lib::leftrotate(
							std::bitset<32>(reg_a.to_ulong() + tempF.to_ulong() + 
							words_table[i-1][p[j]].to_ulong() + k), s[j]
							).to_ulong()
						);
						
				reg_a = tempD;
	/*			std::cout<<"Round = "<<j<<" "<<hash_lib::decimal_to_hex(reg_a.to_ulong())								//uncomment, if you wanna see
							  <<"  "<<hash_lib::decimal_to_hex(reg_b.to_ulong())
							  <<"  "<<hash_lib::decimal_to_hex(reg_c.to_ulong())
							  <<"  "<<hash_lib::decimal_to_hex(reg_d.to_ulong())
				<<"  "<<std::endl; */
			}

			A = std::bitset<32>(reg_a.to_ulong()+ A.to_ulong());
			B = std::bitset<32>(reg_b.to_ulong() +B.to_ulong());
			C = std::bitset<32>(reg_c.to_ulong()+ C.to_ulong());
			D = std::bitset<32>(reg_d.to_ulong() +D.to_ulong());
				
		}

		std::string D_ =	hash_lib::decimal_to_hex( D.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
		std::string C_ =	hash_lib::decimal_to_hex( C.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
		std::string B_ =	hash_lib::decimal_to_hex( B.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
		std::string A_ =	hash_lib::decimal_to_hex( A.to_ulong(),hash_lib::decimal_to_hex_system::x86 ,
										hash_lib::output_type::LITTLE_ENDIAN);
										
		std::string str_ret = A_ + B_ + C_ + D_;
		return str_ret;	

	}



	//ROT13, because why not ;)

	std::string ROT13( std::string str )
	{
		for(int i = 0; i < str.size(); i++)
    		{
        		if((int)str[i] > 64 && (int)str[i] < 91 && (int)str[i] - 13 < 65)
        		{
        		    	int s = (int)str[i] - 65;
    	           		int w = 13 - s;
		            	str[i] = 91 - w;
        		}
        		else if((int)str[i] > 96 && (int)str[i] < 123 && (int)str[i] - 13 < 97)
        		{
            			int s2 = (int)str[i] - 97;
            			int w2 = 13 - s2;
            			str[i] = 123 - w2;
        		}
        		else
        		{
            			str[i] = ((int)str[i] - 13);
        		}

    		}

		return str;

	}

	


	//Function created for computing hex value represented as string from given unsigned int
	
	std::string decimal_to_hex( unsigned long long int a, hash_lib::decimal_to_hex_system par, hash_lib::output_type par2 )
	{
		char hex_table[] = {'0','1','2','3','4','5','6','7',
							'8','9','a','b','c','d','e','f'};

		std::string str;

		while( a != 0)
		{
			str += hex_table[a%16];
			a -= (a%16);
			a /= 16;
		}

		std::string ret_str;
		std::string com_str;
		std::string ret_little_endian;
		
		if(par == hash_lib::decimal_to_hex_system::x86)
			{
				for(int i = 0; i < 8-str.size(); i++)
					com_str += '0';
			
			}else if(par == hash_lib::decimal_to_hex_system::x64)
			{
				for(int i = 0; i < 16-str.size(); i++)
					com_str += '0';
			}
			
		for(int i = str.size()-1; i > -1; i--)
				ret_str += str[i];
			
		std::string ret_com_str = com_str + ret_str;
		
		if(par2 == hash_lib::output_type::LITTLE_ENDIAN)
		{
			std::string temp;
			for(int i = ret_com_str.size()-1; i > -1; i -= 2)
			{
				temp += ret_com_str[i-1];
				temp += ret_com_str[i];
				ret_little_endian += temp;
				temp = "";
			}
			return ret_little_endian;
		}
		
		
		return ret_com_str;
		
	}
	
	

	//templates of function use in process of computing circular bitwise rotations (in right or left)

	template<size_t T>
	inline std::bitset<T> rightrotate(std::bitset<T> bits, unsigned bits_numb_rotate)
	{
		std::bitset<T> rotated_bits(bits >> bits_numb_rotate | bits << (T-bits_numb_rotate));
		return rotated_bits;
	}
	


	template<size_t T>
	inline std::bitset<T> leftrotate(std::bitset<T> bits, unsigned bits_numb_rotate)
	{
		std::bitset<T> rotated_bits(bits << bits_numb_rotate | bits >> (T-bits_numb_rotate));
		return rotated_bits;
	}
}
