#include <memory>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>

#define spWay std::shared_ptr<Way> 
#define spCity std::shared_ptr<City>

using namespace std;

vector<string> parse_string(const string& s)
{
    vector<string> res;
    std::istringstream ist(s);
    for (std::string word; ist >> word; res.push_back(word));
    return res;
}

class City
{
private:
    double x;
    double y;
public:
    City()
    {
        x = 0;
        y = 0;
    }
    City(double nx, double ny)
    {
        x = nx;
        y = ny;
    }
    double get_x() { return x; }
    double get_y() { return y; }
    void set_x(double nx) { x = nx; }
    void set_y(double ny) { y = ny; }

    friend bool operator==(const City& c1, const City& c2);
    friend bool operator!=(const City& c1, const City& c2);
};

bool operator==(const City& c1, const City& c2)
{
    return ((c1.x == c2.x) && (c1.y == c2.y));
}

bool operator!=(const City& c1, const City& c2)
{
    return ((c1.x != c2.x) && (c1.y != c2.y));
}

class Way
{
private:
    vector <spCity> cities;
    double length;
public:
    Way(const vector <spCity>& All_Cities, bool No_Shuffle) //
    {
        int acs = All_Cities.size();

        cities = All_Cities;

        if (!No_Shuffle) {
            for (int i = acs - 1; i >= 0; i--)
            {
                int j = (rand() % acs);
                swap(cities[i], cities[j]);
            }
        }



        length = 0;

        double x1 = cities[0]->get_x();
        double y1 = cities[0]->get_y();

        for (int i = 0; i < cities.size() - 1; i++)
        {

            double x2 = cities[i + 1]->get_x();
            double y2 = cities[i + 1]->get_y();
            length += (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
            x1 = x2;
            y1 = y2;
        }

        double x2 = cities[0]->get_x();
        double y2 = cities[0]->get_y();
        length += (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

    }

    double get_length() { return length; }
    void set_length(double l) { length = l; }
    spCity get_city(int n) { return cities[n]; }
    int get_cities_amount() { return cities.size(); }
    vector <spCity> directCitiesAccess() { return cities; }

    void update_length()
    {
        length = 0;

        double x1 = cities[0]->get_x();
        double y1 = cities[0]->get_y();

        for (int i = 0; i < cities.size() - 1; i++)
        {
            double x2 = cities[i + 1]->get_x();
            double y2 = cities[i + 1]->get_y();
            length += (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
            x1 = x2;
            y1 = y2;
        }
        double x2 = cities[0]->get_x();
        double y2 = cities[0]->get_y();
        length += (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
        if (length == 0) cout << "WARNING SOMETHING BROKE" << endl;
    }
    void swap_two_cities(int c1, int c2)
    {
        swap(cities[c1], cities[c2]);
        update_length();
    }

    void output_cities()
    {
        vector<spCity>::iterator i1 = cities.begin();
        while (i1 != cities.end())
        {
            cout << "(" << (*i1)->get_x() << ", " << (*i1)->get_y() << ") ";
            i1++;
        }
        cout << endl;
    }
    void output_length() { cout << "length is: " << length << endl; }
    void output_way()
    {
        output_cities();
        output_length();
    }

    friend bool operator> (const Way& w1, const Way& w2)
    {
        return w1.length > w2.length;
    }
    friend bool operator< (const Way& w1, const Way& w2)
    {
        return w1.length < w2.length;
    }


};

bool compare(Way* w1, Way* w2)
{
    return w1->get_length() < w2->get_length();
}

bool compare2(spWay w1, spWay w2)
{
    return w1->get_length() < w2->get_length();
}

void GenerateFirstGeneration(int amount, const vector <spCity>& All_Cities, vector <spWay>& Population)
{
    for (int i = 0; i < 2 * amount; i++)
    {
        spWay w(new Way(All_Cities, false));
        Population.push_back(w);

    }
}

Way* Mutation(Way* wA)
{
    int N = wA->get_cities_amount();
    int allele1, allele2;

    allele1 = rand() % N;
    allele2 = rand() % N;
    while (allele1 == allele2) { allele1 = rand() % N; }

    wA->swap_two_cities(allele1, allele2);
    return wA;
}

Tima Voronov, [01.06.2022 22:35]
spWay Mutation2(spWay wA)
{
    int N = wA->get_cities_amount();
    int allele1, allele2;

    allele1 = rand() % N;
    allele2 = rand() % N;
    while (allele1 == allele2) { allele1 = rand() % N; }

    wA->swap_two_cities(allele1, allele2);
    return wA;
}


pair<spWay, spWay> Crossingover2_2(spWay wA, spWay wB, int N)
{
    int mib = (N / 3) + 1;
    int mie = (2 * N / 3) - 1;
    vector <spCity> CA = wA->directCitiesAccess();
    vector <spCity> CB = wB->directCitiesAccess();
    vector <spCity> resA(N);
    vector <spCity> resB(N);
    vector <spCity> rearrangedA;
    vector <spCity> rearrangedB;
    for (int i = mib; i <= mie; ++i) { resA[i] = CA[i]; resB[i] = CB[i]; }
    for (int i = mie + 1; i < N; ++i) { rearrangedA.push_back(CA[i]); rearrangedB.push_back(CB[i]); }
    for (int i = 0; i <= mie; ++i) { rearrangedA.push_back(CA[i]); rearrangedB.push_back(CB[i]); }
    for (int i = 0; i < N; ++i)
    {
        for (int j = mib; j <= mie; ++j)
        {
            if (rearrangedA[i] == resB[j])
            {
                rearrangedA[i] = NULL;
            }
        }
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = mib; j <= mie; ++j)
        {
            if (rearrangedB[i] == resA[j])
            {
                rearrangedB[i] = NULL;
            }
        }
    }

    int ires = 0;
    int irear = 0;

    while (ires != N && irear != N)
    {
        while (rearrangedA[irear] == NULL) irear++;
        while (resB[ires] != NULL) ires++;

        resB[ires] = rearrangedA[irear];
        irear++;
        ires++;
    }

    ires = 0;
    irear = 0;

    while (ires != N && irear != N)
    {
        while (rearrangedB[irear] == NULL) irear++;
        while (resA[ires] != NULL) ires++;

        resA[ires] = rearrangedB[irear];
        irear++;
        ires++;
    }
    spWay nWayA(new Way(resA, true));
    spWay nWayB(new Way(resB, true));

    nWayA = Mutation2(nWayA);
    nWayB = Mutation2(nWayB);

    return make_pair(nWayA, nWayB);
}

vector <spWay> Selection(vector <spWay>& Population)
{
    int N2 = Population.size();
    int N = N2 / 2;
    vector <spWay> res;

    int s1, s2;
    for (int i = 0; i < N; i++)
    {
        s1 = rand() % N / 2;
        s2 = rand() % N / 2;
        while (s1 == s2) { s1 = rand() % N / 2; }
        pair<spWay, spWay> p = Crossingover2_2(Population[s1], Population[s2], N);
        res.push_back(p.first);
        res.push_back(p.second);
    }


    sort(res.begin(), res.end(), compare2);
    return res;

}

int main()
{
    int total_time = 0;
    int curtime = 0;
    int curtimedif = 0;
    string filename;
    vector <spCity> All_Cities;
    vector <spWay> Population;
    int amount;
    int MaxGenerationCount = 0;


    cout << "Input filename: ";
    cin >> filename;

    cout << "Input max generations amount: ";
    cin >> MaxGenerationCount;

    curtime = clock();
    ifstream myFile;
    myFile.open(filename, ios::in | ios::binary);
    string str;
    vector<string> inputstrings;
    getline(myFile, str);
    amount = stoi(str);
    while (getline(myFile, str))
    {
        vector <string> tmp = parse_string(str);
        spCity c(new City(atof(tmp[0].c_str()), atof(tmp[1].c_str())));
        All_Cities.push_back(c);
    }

    curtimedif = clock() - curtime;
    total_time += curtimedif;
    cout << "Input time is: " << curtimedif << " milliseconds" << endl;

    curtime = clock();
    srand(time(NULL));

    GenerateFirstGeneration(amount, All_Cities, Population);

    curtimedif = clock() - curtime;
    total_time += curtimedif;
    cout << "Generated 1st popuation in: " << curtimedif << " milliseconds" << endl;
    curtime = clock();
    sort(Population.begin(), Population.end(), compare2);
    curtimedif = clock() - curtime;
    total_time += curtimedif;
    cout << "sorted in: " << curtimedif << " milliseconds" << endl;

    curtime = clock();
    int GenCount = 1;
    while (GenCount < MaxGenerationCount && clock() < 1800000)
    {
        Population = Selection(Population);
        GenCount++;
    }
    curtimedif = clock() - curtime;
    total_time += curtimedif;

    cout << "FINAL ANSWER IS ";
    Population[0]->output_way();
    cout << "DONE IN " << ((double)total_time) / 1000 << " seconds" << endl;

    system("Pause");
    return 0;

}
