rule kkrunchy
{
	strings:
		$str0={51 8b 45 08 c1 e8 0b 8b 4d 00 0f af 03 8b 09 0f c9 2b 4d 04 39 c8 8b 4d 0c 76 11 }
		$str1={ff 45 00 c1 65 08 08 c1 65 04 08 }
		$str2={89 45 08 31 c0 b4 08 2b 03 d3 e8 01 03 31 c0 eb 0f }
		$str3={01 45 04 29 45 08 8b 03 d3 e8 29 03 83 c8 ff }
		$str4={3d 00 08 00 00 83 d9 ff 83 f8 60 83 d9 ff }
		$str5={41 91 8d 9d a0 08 00 00 e8 b3 00 00 00 }
	condition:
		$str0 and $str1 and $str2 and $str3 and $str4 and $str5
}