rule upx
{
	strings:
		$str0={81 fd 00 f3 ff ff 83 d1 01 8d 14 2f 83 fd fc 76 0f }
		$str1={8a 02 42 88 07 47 49 75 f7 }
		$str2={8b 02 83 c2 04 89 07 83 c7 04 83 e9 04 77 f1 }
		$str3={c1 e0 08 8a 06 46 83 f0 ff 74 74 }
		$str4={8b 1e 83 ee fc 11 db 73 e4 }
	condition:
		$str0 or $str1 or $str2 or $str3 or $str4
}
