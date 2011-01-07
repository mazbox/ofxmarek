void replace(string &str, const string &find_what, const string &replace_with)
{
	string::size_type pos=0;
	while((pos=str.find(find_what, pos))!=string::npos)
	{
		str.erase(pos, find_what.length());
		str.insert(pos, replace_with);
		pos+=replace_with.length();
	}
}
