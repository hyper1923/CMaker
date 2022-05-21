#include <iostream>
#include <cstring>
#include <vector>

#define GPP "g++ "
#define GCC "gcc "


const char* help = R"(
Arguments of CMaker:
    --build example.maker

)";
//G++ 1
//GCC 0

struct Project{
    char projectName[32];
    char executableName[32];
    unsigned char buildType = 1;
};

class MakerFile{
public:
    Project project;
    std::vector<std::string> srcDirectories;
    std::vector<std::string> extraArguments;
    std::string systemString = "";
    bool Build(){
        std::cout << "[" << project.projectName << "]" << " Generating arguments..\n\r";
        if(project.buildType == 0){
            printf("[DEBUG] Building GCC Project...\n\r");
            systemString += GCC;
        } else{printf("[DEBUG] Building G++ Project...\n\r"); systemString += GPP;}

        for(auto& b : srcDirectories){
            systemString += b;
        }
        printf("[DEBUG] Source files has been added to compiler.\n\r");
        systemString += " -o ";
        systemString += project.executableName;
        
        for(auto& b : extraArguments){
            systemString += " -";
            systemString += b;
        }

        std::cout << "[DEBUG] Total arguments of the program: " << systemString << "\n";

        int b = system(systemString.c_str());
        printf("[DEBUG] System code: %d\n\r",b);

        switch (b)
        {
        case 256:
            printf("[ERROR] Compiled couldn't find the binary dir.\n\r");
            exit(EXIT_FAILURE);
            break;
        case 1:
            printf("[ERROR] Your code have errors, check your code.\n\r");
            exit(EXIT_FAILURE);
            break;
        case 0:
            printf("[SUCCESS] Successfully compiled your project.\n\r");
            exit(EXIT_SUCCESS);
        }

        return true;
    }
private:

};


bool Parse(const char* makeFile){
    FILE* f = fopen(makeFile,"r");
    MakerFile maker_file;


    std::string fileType (makeFile);
    fileType = fileType.substr(fileType.size() - 6);
    if(fileType != ".maker"){
        printf("[ERROR] This isn't a .maker file!\n\r");
    }

    if(!f){
        printf("An error occured while trying to parse file.\n\r");
        exit(EXIT_FAILURE);
    }

    char c[128];
    while(true){
        int res = fscanf(f,"%s",&c);
        if(res == EOF){
            printf("[DEBUG] End of the file.\n\r");
            break;
        }

        if(strcmp(c,"PROJECT") == 0){
            int res = fscanf(f,"%s",&c);
            printf("[DEBUG] Project label detected.\n\r");
            c[strlen(c)+1] = '\0';
            std::string b(c);
            b.erase(b.begin());
            b.erase(b.end()-1);
            strcpy(maker_file.project.projectName,&b[0]);
            std::cout << "[DEBUG] PROJECT NAME: " << maker_file.project.projectName << std::endl;
        }

        if(strcmp(c,"ADD_SRC") == 0){
            int res = fscanf(f,"%s",&c);
            printf("[DEBUG] Source file detected.\n\r");
            c[strlen(c)+1] = '\0';
            std::string b(c);
            b.erase(b.begin());
            b.erase(b.end()-1);
            maker_file.srcDirectories.push_back(b);
            std::cout << "[DEBUG] Source Directory: " << b << std::endl;
        }

        if(strcmp(c,"BUILD") == 0){
            int res = fscanf(f,"%s",&c);
            printf("[DEBUG] Source file detected.\n\r");
            c[strlen(c)+1] = '\0';
            std::string b(c);
            b.erase(b.begin());
            b.erase(b.end()-1);
            strcpy(maker_file.project.executableName,&b[0]);
            std::cout << "[DEBUG] Executable Name: " << b << std::endl;
            maker_file.Build();
        }

        if(strcmp(c,"ADD_ARG") == 0){
            int res = fscanf(f,"%s",&c);
            printf("[DEBUG] Argument detected.\n\r");
            c[strlen(c)+1] = '\0';
            std::string b(c);
            b.erase(b.begin());
            b.erase(b.end()-1);
            maker_file.extraArguments.push_back(b);
        }



    }

}


int main(int argc, char** argv){
    if(argc < 2){
        printf("This program need more few arguments.\n\r");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i],"--build") == 0){
            Parse(argv[i+1]);
        }
    }

    return EXIT_SUCCESS;
}