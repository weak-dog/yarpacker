rule obsidium
{
	strings:
		$str0={b9 fc ff ff ff 81 82 b8 00 00 00 ab 00 00 00 eb 05 }
		$str1={89 42 04 89 42 10 eb 01 }
	condition:
		$str0 or $str1
}