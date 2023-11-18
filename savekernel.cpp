#include "savekernel.h"

std::string writeLine(std::vector<double> vec)
{
    std::stringstream os;
    int len = vec.size();
    int i = 0;
    for (const auto& v : vec)
    {
        if (i < len-1)
        {
            os << v << ",";
        }
        else
        {
            os << v;
        }
        i++;
    }
    os << "\n";
    return os.str();
}

bool SaveDataToFile(std::string fname, wxGrid* grid)
{
    wxGridTableBase* wxData = grid->GetTable();
    std::vector<std::vector<double>> data;
    int rows = wxData->GetRowsCount();
    int cols = wxData->GetColsCount();

    for (int i = 0; i < wxData->GetRowsCount(); i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < wxData->GetColsCount(); j++)
        {
            try
            {
                auto v = wxData->GetValue(i, j);
                if (wxData->IsEmptyCell(i, j) == false)
                {
                    std::stringstream ss;
                    ss << v;
                    tmp.push_back(std::stod(ss.str()));
                }
            }
            catch (...)
            {
                return false;
            }

        }
        if (tmp.size() != 0)
        {
            data.push_back(tmp);
        }
    }

    std::ofstream myfile(fname);
    if (myfile.is_open())
    {
        myfile << rows << "," << cols << std::endl;
        
        for (const auto& r : data)
        {
            myfile << writeLine(r);
        }

        myfile.close();
    }
    else
    {
        return false;
    }

	return true;
}

bool LoadDataToFile(std::vector<std::vector<double>> obs, wxGrid* grid)
{
    // grid->SetCellValue(3, 3, "green on grey");
    int i = 0;

    for (auto it = obs.begin(); it != obs.end(); it++,i++)
    {
        std::vector<double> tmp = *it;

        for (int j = 0; j < tmp.size(); j++)
        {
            std::stringstream os;
            os << obs[i][j];
            grid->SetCellValue(i,j, os.str());
        }
    }

    return true;
}


int readCSV(std::vector<std::vector<double>>& obs, std::string filename)
{
    std::ifstream myFile;
    myFile.open(filename, std::ios_base::in);
    bool ignoreheader = true;
    int nfields = 0;

    if (myFile.is_open())
    {
        while (myFile.good())
        {

            std::string Line;
            getline(myFile, Line);

            if (ignoreheader == true)
            {
                int start = 0;
                int pos = static_cast<int>(Line.find(',', start));
                std::string tmp = Line.substr(start, pos - start);
                start = pos + 1;
                nfields = stoi(tmp);
                ignoreheader = false;
                continue;
            }

            if (Line.length() == 0)
            {
                break;
            }
            int pos = 0;
            int start = 0;
            std::vector<double> ob;
            int cnt = 0;
            while (pos != -1)
            {
                if (cnt < nfields)
                {
                    pos = static_cast<int>(Line.find(',', start));
                    std::string tmp = Line.substr(start, pos - start);
                    start = pos + 1;
                    double f = stof(tmp);
                    ob.push_back(f);
                    cnt++;
                }
                else
                {
                    break;
                }
            }

            obs.push_back(ob);
        }
        myFile.close();
    }
    else
    {
        return -1;
    }

    return 0;
};

std::vector<double>& getcol(const std::vector<std::vector<double>>& obs, std::vector<double>& vec, int col)
{
    for (const auto& v : obs)
    {
        vec.push_back(v[col]);
    }
    return vec;
}
