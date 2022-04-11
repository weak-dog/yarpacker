rule upx
{
	strings:
		$str0={8b 02 83 c2 04 89 07 83 c7 04 83 e9 04 77 f1 }
		$str1={81 fd 00 f3 ff ff 83 d1 01 8d 14 2f 83 fd fc 76 0f }
		$str2={c1 e0 08 8a 06 46 83 f0 ff 74 74 }
		$str3={8b 5f 04 8d 84 30 dc ?1 02 00 01 f3 50 83 c7 08 ff 96 40 ?2 02 00 }
		$str4={89 f9 57 48 f2 ae 55 ff 96 48 ?2 02 00 }
	condition:
		$str0 and $str1 and $str2 and $str3 and $str4
}