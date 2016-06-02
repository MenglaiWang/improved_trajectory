#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <omp.h>
using namespace std;

#define IDT_dim 435
string get_pre_name(string file_path)
{
    return file_path.substr(file_path.rfind("/") + 1, file_path.rfind(".") - 1 - file_path.rfind("/"));
}

int replace(string in_path)
{
    ifstream in(in_path.c_str());
    vector<string> content;
    string line;
    while (getline(in, line))
        content.push_back(line);
    in.close();

    if (content.size() == 0)
    {
        return -1;
    }

    if (strlen(content[0].c_str()) < 3000 || strlen(content[0].c_str()) > 6000)
    {
        /*
        ofstream out("log.txt",ios::app);
        cout<<in_path<<" is not based on char"<<endl;
        out<<in_path<<" is not based on char"<<endl;
        out.close();
        */
        return -1;
    }
    FILE *stream;
    if ((stream = fopen(in_path.c_str(), "w+")) == NULL)
    {
        fprintf(stderr, "cannot open file.\n");
        return -1;
    }
    int numData = content.size();
    int dim = IDT_dim;
    fwrite(&dim, sizeof(int), 1, stream);
    fwrite(&numData, sizeof(int), 1, stream);
    for (int i = 0; i < content.size(); i++)
    {
        stringstream ss;
        ss << content[i];
        int frame_pos;
        ss >> frame_pos;
        fwrite(&frame_pos, sizeof(int), 1, stream);
        float *feature = (float*)malloc(sizeof(float) * IDT_dim);
        for (int j = 0; j < IDT_dim; j++)
            ss >> feature[j];
        fwrite(feature, sizeof(float), IDT_dim, stream);
        if (feature)
            free(feature);
    }
    fclose(stream);
}

int main(int argc, char** argv)
{
    ifstream in(argv[1]);
    string out_dir = argv[2];
    string line;
    vector<string> video_paths;
    while (getline(in, line))
    {
        stringstream ss;
        ss << line;
        string video_path;
        ss >> video_path;
        video_paths.push_back(video_path);
    }
    #pragma omp parallel for  num_threads(32)
    for (int i = 0; i < video_paths.size(); i++)
    {
        string video_path = video_paths[i];
        string output_path = out_dir + "/" + get_pre_name(video_path) + ".bin";
        if (access(output_path.c_str(), F_OK) != -1)
        {
            cout << output_path << " exsist !" << endl;
            continue;
        }
        system(("../DenseTrackStab -f " + video_path + " -o " + output_path).c_str());
        system("wait");
      //  replace(output_path);
    }

}
