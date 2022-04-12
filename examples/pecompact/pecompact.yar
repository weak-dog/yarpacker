rule pecompact
{
	strings:
		$str0={5a 8b f8 50 52 8b 33 8b 43 20 03 c2 8b 08 89 4b 20 8b 43 1c 03 c2 8b 08 89 4b 1c 03 f2 8b 4b 0c 03 ca 8d 43 1c 50 57 56 ff d1 }
		$str1={5a 58 03 43 08 8b f8 52 8b f0 8b 46 fc 83 c0 04 2b f0 89 56 08 8b 4b 0c 89 4e 14 ff d7 }
		$str2={89 85 3f 13 00 10 8b f0 8b 4b 14 5a eb 0c }
		$str3={2b 7c 24 28 89 7c 24 1c 61 c2 0c 00 }
		$str4={8b c6 5a 5e 5f 59 5b 5d ff e0 }
		$str5={b8 ?? ?? 42 f0 8d 88 9e 12 00 10 89 41 01 8b 54 24 04 8b 52 0c c6 02 e9 83 c2 05 2b ca 89 4a fc 33 c0 c3 }
	condition:
		$str0 and $str1 and $str2 and $str3 and $str4 and $str5
}