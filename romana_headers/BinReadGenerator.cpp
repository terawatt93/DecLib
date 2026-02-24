#include "toptions.h"

string GenerateReadForClass(string ClassName)
{
	stringstream s;
	s<<"void ReadFromBin("<<ClassName<<" &obj, char *buf1,size_t BUF_SIZE)\n{\n";
	s<<"\tstd::istringstream ifs(std::string(buf1, BUF_SIZE));\n\tchar buf[500000];\n\tUShort_t len = 0;\n";
	s<<"\twhile(ifs)\n\t{\n";
	s<<"\t\tifs.read(reinterpret_cast<char*>(&len), sizeof(len));\n\t\tifs.read(reinterpret_cast<char*>(&buf), len);\n\t\tif(len<1){continue;}\n\t\tstring str(buf, len - 1);\n";
	//s<<"cout<<str<<endl;\n";
		
	TList* ll=TClass::GetClass(ClassName.c_str())->GetListOfDataMembers();
	TIter next(ll);
	int member_iter=0;
	while (TDataMember *dm = (TDataMember*)next())
	{
		string mem_name(dm->GetTypeName());
		if(mem_name.find("Hdef")!=string::npos || string(dm->GetName())=="l2d")
		{
			continue;
		}
		if(member_iter==0)
		{
			s<<"\t\tif(str==\""<<dm->GetName()<<"\")\n\t\t{\n";
		}
		else
		{
			s<<"\t\telse if(str==\""<<dm->GetName()<<"\")\n\t\t{\n";
		}
		s<<"\t\t\tifs.read(reinterpret_cast<char*>(&len), sizeof(len));\n";
		//s<<"\t\t\tifs.read(reinterpret_cast<char*>(&obj."<<dm->GetName()<<"), sizeof(obj."<<dm->GetName()<<"));\n";
		s<<"\t\t\tifs.read(reinterpret_cast<char*>(&obj."<<dm->GetName()<<"), len);\n";
		member_iter++;
		s<<"\t\t}\n";
	}
	s<<"\t\telse\n\t\t{\n";
	s<<"\t\t\tifs.read(reinterpret_cast<char*>(&len), sizeof(len));\n\t\t\tifs.read(reinterpret_cast<char*>(&buf), len);\n\t\t}\n";
	s<<"\t}\n}\n";
	return s.str();
}
string GenerateCodeForVector(string vec_name,string vec_type,string class_var_name,vector<int> dimensions,string Indent="\t",int order=0)
{
	cout<<"vec_name: "<<vec_name<<" "<<vec_type<<" "<<dimensions.size()<<"\n";
	stringstream s;
	if(order==0)
	{
		s<<Indent;
		for(int i=0;i<dimensions.size();i++)
		{
			s<<"vector<";
			if(i==dimensions.size()-1)
			{
				s<<vec_type;
			}
		}
		for(int i=0;i<dimensions.size();i++)
		{
			s<<"> ";
		}
		s<<vec_name<<";\n";
	}
	string index_name="i"+to_string(order);
	if(order==0)
	{
		s<<Indent<<vec_name<<".resize("<<dimensions[0]<<");\n";
	}
	if(int(dimensions.size())>order)
	{
		s<<Indent<<"for(int "<<index_name<<"=0;"<<index_name<<"<"<<dimensions[order]<<";"<<index_name<<"++)\n";
		s<<Indent<<"{\n";
		string index;
		index="[i"+to_string(order)+"]";
		
		if(dimensions.size()-1==order)
		{
			string var_name0=vec_name+index;
			size_t pos = var_name0.find('[');
			if (pos != std::string::npos)
			{
				var_name0.replace(0, pos, class_var_name);
			}
			s<<Indent<<"\t"<<vec_name+index<<"="<<var_name0<<";\n";
		}
		else
		{
			s<<Indent<<"\t"<<vec_name<<index<<".resize("<<dimensions[order+1]<<");\n";
			s<<GenerateCodeForVector(vec_name+index,vec_type,class_var_name,dimensions,Indent+"\t",order+1);
		}
		s<<Indent<<"}\n";
	}
	//else 
	return s.str();
}
string GenerateConvertationToJSON(TDataMember *dm,string obj_name="obj", string json_name="jsn", string Indent="\t")
{
	string ClassName=dm->GetClass()->GetName();
	string dat_type=dm->GetTypeName();
	string varname=dm->GetName();
	int dimensions=dm->GetArrayDim();
	stringstream s;
	
	vector<int> dim_vector;
	//s<<ClassName<<":"<<dat_type<<" "<<varname<<" "<<dimensions<<" ";
	for(int i=0;i<dimensions;i++)
	{
		dim_vector.push_back(dm->GetMaxIndex(i));
	}
	
	//дальше должна быть разная логика для разных типов данных
	if(dat_type == "char")//если "char", то это строки
	{
		if(dimensions==0||dimensions==1)
		{
			s<<Indent<<json_name<<"[\""<<varname<<"\"]"<<"=string("<<obj_name<<"."<<varname<<");\n";
		}
		else
		{
			dim_vector.resize(dim_vector.size()-1);
			s<<GenerateCodeForVector(varname+"_vec","string",obj_name+"."+varname,dim_vector,Indent);
			s<<Indent<<json_name<<"[\""<<varname<<"\"]="<<varname+"_vec;\n";
		}
	}
	else
	{
		if(dimensions==0)
		{
			s<<Indent<<json_name<<"[\""<<varname<<"\"]="<<obj_name<<"."<<varname<<";\n";
		}
		else
		{
			s<<GenerateCodeForVector(varname+"_vec",dat_type,obj_name+"."+varname,dim_vector,Indent);
			s<<Indent<<json_name<<"[\""<<varname<<"\"]="<<varname+"_vec;\n";
		}
	}
	return s.str();
}

string GenerateJSONForClass(string ClassName)
{
	stringstream s;
	s<<"void ToJSON("<<ClassName<<" &obj, nlohmann::json &jsn)\n{\n";
	TList* ll=TClass::GetClass(ClassName.c_str())->GetListOfDataMembers();
	TIter next(ll);
	int member_iter=0;
	s<<"\tjsn[\"Class\"]=\""<<ClassName<<"\";\n";
	while (TDataMember *dm = (TDataMember*)next())
	{
		string mem_name(dm->GetTypeName());
		if(mem_name.find("Hdef")!=string::npos || string(dm->GetName())=="l2d" || string(dm->GetName())=="device" || string(dm->GetName())=="ver_po")
		{
			continue;
		}
		s<<GenerateConvertationToJSON(dm);
		//cout<<GenerateConvertationToJSON(dm);
		/*string mem_name(dm->GetTypeName());
		if(mem_name.find("Hdef")!=string::npos || string(dm->GetName())=="l2d")
		{
			continue;
		}
		cout<<"type: "<<dm->GetTypeName()<<" varname:"<<dm->GetName()<<" size:"<<dm->GetArrayDim()<<"\n";
		s<<"\tcout<<\""<<dm->GetName()<<"\"<<endl;\n";
		s<<"\tjsn[\""<<dm->GetName()<<"\"]=obj."<<dm->GetName()<<";\n";*/
	}
	s<<"}\n";
	return s.str();
}

void BinReadGenerator()
{
	TList* ll=TClass::GetClass("Coptions")->GetListOfDataMembers();
	TIter next(ll);
	while (TDataMember *dm = (TDataMember*)next()) 
	{ cout << "Member: " << dm->GetName() <<" "<<dm->GetTrueTypeName() <<" "<<dm->GetArrayDim() << endl; }
	ofstream ofs1("BinRead.cpp");
	ofs1<<"#include \"toptions.h\"\n#include <iostream>\n#include <fstream>\n#include <sstream>\n#include <nlohmann/json.hpp>\n/*This code is generated by BinReadGenerator.cpp using classes stored in \"toptions.h\" file.\nDo not change them!*/\n";
	ofs1<<GenerateReadForClass("Coptions");
	ofs1<<GenerateReadForClass("Toptions");
	ofs1<<GenerateJSONForClass("Coptions")<<GenerateJSONForClass("Toptions");
}
