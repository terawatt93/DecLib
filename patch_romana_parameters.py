#этот код нужен для извлечения заголовочных файлов toptions.h и common.h из romana для чтения параметров из dec

import sys
import clang.cindex
import os

def find_paths_with_romana(ignore_case=True):
    """
    Возвращает список путей из переменной окружения PATH,
    которые содержат подстроку '/romana/' или '/romana'.
    При ignore_case=True поиск регистронезависимый.
    """
    path_env = os.environ.get('PATH', '')
    if not path_env:
        return []

    paths = path_env.split(os.pathsep)
    results = []

    # Подстроки для поиска (в нижнем регистре)
    needle1 = '/romana/'
    needle2 = '/romana'

    for p in paths:
        p_stripped = p.strip()
        if not p_stripped:
            continue
        # Приводим к нижнему регистру для регистронезависимого сравнения
        p_lower = p_stripped.lower() if ignore_case else p_stripped
        if needle1 in p_lower or needle2 in p_lower:
            results.append(p_stripped)  # сохраняем оригинальный путь
    return results

#remove_constructors_destructors сгенерирован deepseek
def remove_constructors_destructors(input_file, output_file=None):
    # Парсим файл
    # Читаем исходный код в память
    f=open(input_file, 'r', encoding='utf-8')
    code = f.readlines()
    f.close()
    #сначала извлечем имена классов
    Classes=[]
    for l in code:
        #print(l)
        #print("aaaaaa")
        if "class " in l:
            l=l.replace("class ","")
            l=l.replace("{","")
            l=l.replace(" ","")
            l=l.replace("\t","")
            l=l.replace("\n","")
            colon_pos=l.find(":")
            if colon_pos > 0 :
                l=l[0:colon_pos]
            if l not in Classes:
                Classes.append(l)
    #теперь удаляем строки, содержащие признаки конструкторов и деструкторов () и пишем все это в output_file
    f_out=open(output_file,"w")
    for l in code:
        
        l=l.replace("private:","public:")
        Write=True
        for classname in Classes:
            l_cut=l.split("//")[0]
            if (classname in l_cut and ("(" in l_cut or "~" in l_cut)) or ")" in l_cut:
                Write=False
        if Write:
            f_out.write(l)
    print("patched file saved in "+output_file)
    

#ll=TClass::GetClass("Coptions")->GetListOfDataMembers()
#TIter next(ll); while (TDataMember *dm = (TDataMember*)next()) { cout << "Member: " << dm->GetName() <<" "<<dm->GetTrueTypeName() <<" "<<dm->GetArrayDim() << endl; }


def generate_headers_for_parameter():
    path_to_romana=find_paths_with_romana()
    if len(path_to_romana) == 0:
        print("Ошибка: в PATH отсутствует путь к исходникам romana (возможно, путь прописан к  bin?)")
    if os.path.exists(path_to_romana[0]+"/src/toptions.h"):
        os.system("cp "+path_to_romana[0]+"/src/toptions.h romana_headers/")
        os.system("cp "+path_to_romana[0]+"/src/common.h romana_headers/")
        remove_constructors_destructors("romana_headers/toptions.h","romana_headers/toptions.h")
        os.system("cd romana_headers; root BinReadGenerator.cpp")
    else:
        print("Невозможно сгенерировать хедеры для чтения параметров: в "+path_to_romana[0]+"/src отсутствует toptions.h")


if __name__ == '__main__':
    generate_headers_for_parameter()
