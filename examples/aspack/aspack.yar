rule aspack
{
	strings:
		$str0={8b d0 2b d1 40 8a 12 88 50 ff 8b 16 3b c2 72 f0 }
		$str1={8b 44 24 08 8b d1 8b 4c 24 04 57 89 02 8d 42 04 89 08 c7 40 04 20 00 00 00 89 42 10 89 82 a0 00 00 00 89 82 30 01 00 00 89 82 c0 01 00 00 33 c0 b9 bd 00 00 00 89 82 50 02 00 00 89 82 54 02 00 00 89 82 58 02 00 00 8b ba 60 02 00 00 89 82 5c 02 00 00 f3 ab 8b ca aa e8 04 00 00 00 }
		$str2={8b 75 00 8b 44 24 10 8b 5c 24 1c 8b ba 8c 00 00 00 c1 ee 10 8b ce 25 ff 00 00 00 2b cb 03 fb 8a d8 8b d1 8a fb 89 74 24 1c 8b c3 8b 74 24 14 c1 e0 10 66 8b c3 c1 e9 02 f3 ab 8b ca 8b 54 24 20 83 e1 03 f3 aa 8b 7c 24 24 8b 4c 24 18 }
		$str3={8b 0e 8b 79 04 03 fa 89 79 04 8b 1c 96 b9 18 00 00 00 2b c3 2b ca 5f d3 e8 8b 4c 96 44 03 c1 8b 8e 88 00 00 00 5e 5b 8b 04 81 59 c3 }
		$str4={8b 50 04 8b 40 08 b9 08 00 00 00 2b ca d3 e8 8b 4e 24 25 00 fe ff 00 3b c1 73 14 }
		$str5={8b 46 08 8b 7e 0c b9 08 00 00 00 2b c8 03 c5 d3 ef b9 18 00 00 00 89 46 08 2b cd 81 e7 ff ff ff 00 d3 ef 8d 8e 30 01 00 00 e8 1b fb ff ff }
		$str6={81 ec 98 00 00 00 53 55 56 8b d1 57 b9 0f 00 00 00 8b aa 84 00 00 00 33 c0 8d 7c 24 2c 33 f6 f3 ab 8b bc 24 ac 00 00 00 3b ee 89 54 24 20 76 15 }
		$str7={8b c8 8b 3e 03 bd a0 04 00 00 8b b5 50 01 00 00 c1 f9 02 f3 a5 8b c8 83 e1 03 f3 a4 5e 68 00 80 00 00 6a 00 ff b5 50 01 00 00 ff 55 5e }
		$str8={8b ba 88 00 00 00 25 ff 00 00 00 8b 44 84 68 89 0c 87 33 c0 8a 04 31 8b 7c 84 68 8d 44 84 68 47 89 38 }
		$str9={b9 17 00 00 00 89 74 24 28 89 72 04 89 72 44 89 74 24 68 33 ff 89 74 24 1c c7 44 24 10 01 00 00 00 89 4c 24 18 8d 6a 08 89 74 24 14 }
	condition:
		$str0 and $str1 and $str2 and $str3 and $str4 and $str5 and $str6 and $str7 and $str8 and $str9
}