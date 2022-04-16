rule upx
{
	strings:
		$str0={8a 02 42 88 07 47 49 75 f7 }
		$str1={8b 02 83 c2 04 89 07 83 c7 04 83 e9 04 77 f1 }
		$str2={81 fd 00 f3 ff ff 83 d1 01 8d 14 2f 83 fd fc 76 0f }
		$str3={8b ae 4c ?2 02 00 8d be 00 f0 ff ff bb 00 10 00 00 50 54 6a 04 53 57 ff d5 }
		$str4={c1 e0 08 8a 06 46 83 f0 ff 74 74 }
		$str5={8b 1e 83 ee fc 11 db 73 e4 }
		$str6={8b 5f 04 8d 84 30 dc ?1 02 00 01 f3 50 83 c7 08 ff 96 40 ?2 02 00 }
		$str7={89 f9 57 48 f2 ae 55 ff 96 48 ?2 02 00 }
		$str8={8d 87 ?f 0? 00 00 80 20 7f 80 60 28 7f 58 50 54 50 53 57 ff d5 }
		$str9={60 be 00 ?0 42 00 8d be 00 ?0 fe ff 57 83 cd ff eb 10 }
	condition:
		$str0 or $str1 or $str2 or $str3 or $str4 or $str5 or $str6 or $str7 or $str8 or $str9
}
