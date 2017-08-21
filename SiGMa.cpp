#include<iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <string>

using namespace std;
struct unit_sim {
	long index_i;
	long index_j;
	double sim;
	bool movie = 1;//1代表是电影 0代表是人物
	unit_sim(long n1, long n2, double s,bool isM) {
		index_i = n1;
		index_j = n2;
		sim = s;
		movie = isM;
	}
};

struct unit_comp {
	bool operator()(const unit_sim &na, const unit_sim &nb) {
		return na.sim < nb.sim;
	}
};

class SiGMA {
	string file1, file2, file3, file4, file5, file6, file7, file8, file9;
	vector<string> movieIdList1, movieIdList2, personIdList1, personIdList2;
	vector<vector<pair<long, double>>> movieSim, personSim;
	vector<vector<pair<long, double>>> movieSim_yago, personSim_yago;
	map<long, long> matched_pairs;
	map<long, long> truth,truth_yago;
	priority_queue<unit_sim, vector<unit_sim>, unit_comp> pairs_queue;
	long imdb_movie_size, imdb_person_size, yago_movie_size, yago_person_size;
	vector<vector<pair<long,int>>> imdb_movie_person, yago_movie_person, imdb_person_movie, yago_person_movie;
	double threshold_low, threshold_high;

public:
	double checkSim(long index_i, long index_j, bool isM) {
		//index_i imdb index_j yago
		//0 directs 1 acted in 2 write
		double sim = 0;
		if (isM) {
			for (int i = 0;i < movieSim[index_i].size();i++) {
				if (movieSim[index_i][i].first == index_j)sim = movieSim[index_i][i].second;
			}
			vector<long> actor_imdb, actor_yago;
			vector<long> director_imdb, director_yago;
			vector<long> writer_imdb, writer_yago;
			for (int i = 0;i < imdb_movie_person[index_i].size();i++) {
				if (imdb_movie_person[index_i][i].second == 0)director_imdb.push_back(imdb_movie_person[index_i][i].first);
				else if (imdb_movie_person[index_i][i].second == 1)actor_imdb.push_back(imdb_movie_person[index_i][i].first);
				else writer_imdb.push_back(imdb_movie_person[index_i][i].first);
			}
			for (int i = 0;i < yago_movie_person[index_j].size();i++) {
				if (yago_movie_person[index_j][i].second == 0)director_yago.push_back(yago_movie_person[index_j][i].first);
				else if (yago_movie_person[index_j][i].second == 1)actor_yago.push_back(yago_movie_person[index_j][i].first);
				else writer_yago.push_back(yago_movie_person[index_j][i].first);
			}
			vector<double>actor, director, writer;
			for (int i = 0;i < actor_imdb.size();i++) {
				if (personSim[actor_imdb[i]].size() == 0)continue;
				for (int j = 0;j < personSim[actor_imdb[i]].size();j++) {
					for (int k = 0;k < actor_yago.size();k++) {
						if (actor_yago[k] == personSim[actor_imdb[i]][j].first)actor.push_back(personSim[actor_imdb[i]][j].second);
					}
				}
			}
			for (int i = 0;i < director_imdb.size();i++) {
				if (personSim[director_imdb[i]].size() == 0)continue;
				for (int j = 0;j < personSim[director_imdb[i]].size();j++) {
					for (int k = 0;k < director_yago.size();k++) {
						if (director_yago[k] == personSim[director_imdb[i]][j].first)director.push_back(personSim[director_imdb[i]][j].second);
					}
				}
			}
			for (int i = 0;i < writer_imdb.size();i++) {
				if (personSim[writer_imdb[i]].size() == 0)continue;
				for (int j = 0;j < personSim[writer_imdb[i]].size();j++) {
					for (int k = 0;k < writer_yago.size();k++) {
						if (writer_yago[k] == personSim[writer_imdb[i]][j].first)writer.push_back(personSim[writer_imdb[i]][j].second);
					}
				}
			}
			double a = 0, d = 0, w = 0;
			if (actor.size() != 0) {
				for (int i = 0;i < actor.size();i++)a += actor[i];
				a /= actor.size();
			}
			if (director.size() != 0) {
				for (int i = 0;i < director.size();i++)d += director[i];
				d /= director.size();
			}
			if (writer.size() != 0) {
				for (int i = 0;i < writer.size();i++)w += writer[i];
				w /= writer.size();
			}
			cout << "sim = " << sim << endl;
			cout<<"relationSIm= "<<a + d + w<<endl;
		}
		else {
			for (int i = 0;i < personSim[index_i].size();i++) {
				if (personSim[index_i][i].first == index_j)sim = personSim[index_i][i].second;
			}
			vector<long> movie_imdb, movie_yago;
			for (int i = 0;i < imdb_person_movie[index_i].size();i++) {
				movie_imdb.push_back(imdb_person_movie[index_i][i].first);
			}
			for (int i = 0;i < yago_person_movie[index_j].size();i++) {
				movie_yago.push_back(yago_person_movie[index_j][i].first);
			}
			vector<double>movie;
			for (int i = 0;i < movie_imdb.size();i++) {
				if (movieSim[movie_imdb[i]].size() == 0)continue;
				for (int j = 0;j < movieSim[movie_imdb[i]].size();j++) {
					for (int k = 0;k < movie_yago.size();k++) {
						if (movie_yago[k] == movieSim[movie_imdb[i]][j].first)movie.push_back(movieSim[movie_imdb[i]][j].second);
					}
				}
			}

			double m = 0;
			if (movie.size() != 0) {
				for (int i = 0;i < movie.size();i++)m += movie[i];
				m /= movie.size();
			}
			cout << "Sim= " << sim << endl;
			cout << "relationSim: "<<m / 3 << endl;
		}
		return sim;
	}

	double reCalSim(long index_i,long index_j,bool isM) {
		//index_i imdb index_j yago
		//0 directs 1 acted in 2 write
		double res = 0;
		double sim = 0;
		double alpha = double(2) / 3;
		if (isM) {
			for (int i = 0;i < movieSim[index_i].size();i++) {
				if (movieSim[index_i][i].first == index_j)sim = movieSim[index_i][i].second;
			}
			vector<long> actor_imdb, actor_yago;
			vector<long> director_imdb, director_yago;
			vector<long> writer_imdb, writer_yago;
			for (int i = 0;i < imdb_movie_person[index_i].size();i++) {
				if (imdb_movie_person[index_i][i].second == 0)director_imdb.push_back(imdb_movie_person[index_i][i].first);
				else if (imdb_movie_person[index_i][i].second == 1)actor_imdb.push_back(imdb_movie_person[index_i][i].first);
				else writer_imdb.push_back(imdb_movie_person[index_i][i].first);
			}
			for (int i = 0;i < yago_movie_person[index_j].size();i++) {
				if (yago_movie_person[index_j][i].second == 0)director_yago.push_back(yago_movie_person[index_j][i].first);
				else if (yago_movie_person[index_j][i].second == 1)actor_yago.push_back(yago_movie_person[index_j][i].first);
				else writer_yago.push_back(yago_movie_person[index_j][i].first);
			}
			vector<double>actor, director, writer;
			for (int i = 0;i < actor_imdb.size();i++) {
				if (personSim[actor_imdb[i]].size() == 0)continue;
				for (int j = 0;j < personSim[actor_imdb[i]].size();j++) {
					for (int k = 0;k < actor_yago.size();k++) {
						if (actor_yago[k] == personSim[actor_imdb[i]][j].first)actor.push_back(personSim[actor_imdb[i]][j].second);
					}
				}
			}
			for (int i = 0;i < director_imdb.size();i++) {
				if (personSim[director_imdb[i]].size() == 0)continue;
				for (int j = 0;j < personSim[director_imdb[i]].size();j++) {
					for (int k = 0;k < director_yago.size();k++) {
						if (director_yago[k] == personSim[director_imdb[i]][j].first)director.push_back(personSim[director_imdb[i]][j].second);
					}
				}
			}
			for (int i = 0;i < writer_imdb.size();i++) {
				if (personSim[writer_imdb[i]].size() == 0)continue;
				for (int j = 0;j < personSim[writer_imdb[i]].size();j++) {
					for (int k = 0;k < writer_yago.size();k++) {
						if (writer_yago[k] == personSim[writer_imdb[i]][j].first)writer.push_back(personSim[writer_imdb[i]][j].second);
					}
				}
			}
			double a=0, d=0, w=0;
			if (actor.size() != 0) {
				for (int i = 0;i < actor.size();i++)a += actor[i];
				a /= actor.size();
			}
			if (director.size() != 0) {
				for (int i = 0;i < director.size();i++)d += director[i];
				d /= director.size();
			}
			if (writer.size() != 0) {
				for (int i = 0;i < writer.size();i++)w += writer[i];
				w /= writer.size();
			}
			double relationSim = a + d + w;
			if (relationSim - 0.3 >= sim)res = sim / 3 + relationSim*2 /3;
			else res = (sim*2)/3 + relationSim / 3;
		}
		else {
			for (int i = 0;i < personSim[index_i].size();i++) {
				if (personSim[index_i][i].first == index_j)sim = personSim[index_i][i].second;
			}
			sim = sim * alpha;
			vector<long> movie_imdb, movie_yago;
			for (int i = 0;i < imdb_person_movie[index_i].size();i++) {
				movie_imdb.push_back(imdb_person_movie[index_i][i].first);
			}
			for (int i = 0;i < yago_person_movie[index_j].size();i++) {
				movie_yago.push_back(yago_person_movie[index_j][i].first);
			}
			vector<double>movie;
			for (int i = 0;i < movie_imdb.size();i++) {
				if (movieSim[movie_imdb[i]].size() == 0)continue;
				for (int j = 0;j < movieSim[movie_imdb[i]].size();j++) {
					for (int k = 0;k < movie_yago.size();k++) {
						if (movie_yago[k] == movieSim[movie_imdb[i]][j].first)movie.push_back(movieSim[movie_imdb[i]][j].second);
					}
				}
			}
			
			double m=0;
			if (movie.size() != 0) {
				for (int i = 0;i < movie.size();i++)m += movie[i];
				m /= movie.size();
			}
			res = sim + m / 3;
		}
		return res;
	}

	void InitializeMovieIdList() {
		cout << "InitializeMovieIdList" << endl;
		ifstream in1, in2;
		in1.open(file1);
		in2.open(file2);

		string readline;
		while (getline(in1, readline)) {
			//cout << readline << endl;
			int index = readline.find_first_of(',', 0);
			movieIdList1.push_back(readline.substr(index+1, readline.length()));
		}
		while (getline(in2, readline)) {
			int index = readline.find_first_of(',', 0);
			movieIdList2.push_back(readline.substr(index+1, readline.length()));
		}
		imdb_movie_size = movieIdList1.size();
		yago_movie_size = movieIdList2.size();
	}

	void InitializePersonIdList() {
		cout << "InitializePersonIdList" << endl;
		ifstream in1, in2;
		in1.open(file3);
		in2.open(file4);

		string readline;
		while (getline(in1, readline)) {
			int index = readline.find_first_of(',', 0);
			personIdList1.push_back(readline.substr(index, readline.length()));
		}
		while (getline(in2, readline)) {
			int index = readline.find_first_of(',', 0);
			personIdList2.push_back(readline.substr(index, readline.length()));
		}
		imdb_person_size = personIdList1.size();
		yago_person_size = personIdList2.size();
	}


	void InitializeSim() {
		cout << "InitializeSim" << endl;
		ifstream in1, in2;
		in1.open(file5);
		in2.open(file6);
		string readline;

		for (int i = 0;i < imdb_movie_size;i++) {
			vector<pair<long, double>> t;
			movieSim.push_back(t);
		}
		for (int i = 0;i < imdb_person_size;i++) {
			vector<pair<long, double>> t;
			personSim.push_back(t);
		}
		for (int i = 0;i < yago_movie_size;i++) {
			vector<pair<long, double>> t;
			movieSim_yago.push_back(t);
		}

		while (getline(in1, readline)) {
			int index = readline.find_first_of(',', 0), index1 = readline.find_first_of(',', index + 1), index2 = readline.find_first_of(',', index1 + 1);
			string s1 = "", s2 = "", s3 = "";
			for (int i = 0;i < readline.length();i++) {
				if (0 <= i && i < index)s1 += readline[i];
				else if (i>index &&i < index1)s2 += readline[i];
				else if (i>index1 &&i < index2)s3 += readline[i];
			}
			char c1[100], c2[100], c3[100];
			strcpy(c1, s1.c_str());
			strcpy(c2, s2.c_str());
			strcpy(c3, s3.c_str());
			long n1 = atol(c1), n2 = atol(c2);
			double n3 = atof(c3);
			pair<long, double > p(n2, n3);
			movieSim[n1].push_back(p);

			pair<long, double> p1(n1, n3);
			movieSim_yago[n2].push_back(p1);
		}
		while (getline(in2, readline)) {
			int index = readline.find_first_of(',', 0), index1 = readline.find_first_of(',', index + 1), index2 = readline.find_first_of(',', index1 + 1);
			string s1 = readline.substr(0, index);
			string s2 = readline.substr(index + 1, index1);
			string s3 = readline.substr(index1 + 1, index2);
			char c1[100], c2[100], c3[100];
			strcpy(c1, s1.c_str());
			strcpy(c2, s2.c_str());
			strcpy(c3, s3.c_str());
			long n1 = atol(c1), n2 = atol(c2);
			double n3 = atof(c3);
			pair<long, double > p(n2, n3);
			personSim[n1].push_back(p);
		}

	}

	void Initialize_matched_pair() {
		cout << "Initialize_matched_pair" << endl;
		int count = 0;
		for (int i = 0;i < movieSim.size();i++) {
			if (movieSim[i].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim[i].size();j++) {
				if (movieSim[i][j].second > max) {
					max = movieSim[i][j].second;
					index = movieSim[i][j].first;
				}
			}
			//cout << max << " ";
			if (max >= 0.5) { 
				//cout << "max=" << max << endl;
				matched_pairs.insert(pair<long, long>(i, index));
				count++;
			}
		}
		cout << "The amount of initial matched pairs is " << count << endl;
	}


	long traceMovie(string name,bool isIMDB) {
		long res = -1;
		if (isIMDB) {
			for (long i = 0;i < movieIdList1.size();i++) {
				if (name == movieIdList1[i])return i;
			}
		}
		else {
			for (long i = 0;i < movieIdList2.size();i++) {
				if (name == movieIdList2[i])return i;
			}
		}
		return res;
	}

	void initializeTruth() {
		ifstream in;
		in.open(file9);
		string readline = "";
		while (getline(in, readline)) {
			string s1 = "", s2 = "";
			int index = 0;
			for (;readline[index] != ',';index++) s1 += readline[index];
			index++;
			for (;index < readline.length();index++) s2 += readline[index];
			char c1[100], c2[100];
			strcpy(c1, s1.c_str());
			strcpy(c2, s2.c_str());
			long n1 = atol(c1), n2 = atol(c2);
			truth.insert(pair<long, long>(n1, n2));
			truth_yago.insert(pair<long, long>(n2, n1));
		}
	}

	long find_in_truth(long index, bool isIMDB) {
		map<long, long>::iterator it;
		if (isIMDB) {
			it = truth.find(index);
			if (it == truth.end())return -1;
			return it->second;
		}
		else {
			it = truth_yago.find(index);
			if (it == truth_yago.end())return -1;
			return it->second;
		}
	}

	void Initialize_matched_pair_from_SCAN() {
		ifstream in1, in2, in3, in4,in5,in6;
		in1 .open("C:\\Users\\Administrator\\PycharmProjects\\CommunityDetection\\algorithm\\SCAN_hubs&outliers\\imdb_hubs.txt");
		in2.open("C:\\Users\\Administrator\\PycharmProjects\\CommunityDetection\\algorithm\\SCAN_hubs&outliers\\imdb_isolated_4time.txt");
		in3.open("C:\\Users\\Administrator\\PycharmProjects\\CommunityDetection\\algorithm\\SCAN_hubs&outliers\\yago_hubs.txt");
		in4.open("C:\\Users\\Administrator\\PycharmProjects\\CommunityDetection\\algorithm\\SCAN_hubs&outliers\\yago_isolated_4time.txt");
		in5.open("C:\\Users\\Administrator\\PycharmProjects\\CommunityDetection\\algorithm\\SCAN_hubs&outliers\\imdb_community_4time.txt");
		in6.open("C:\\Users\\Administrator\\PycharmProjects\\CommunityDetection\\algorithm\\SCAN_hubs&outliers\\yago_community_4time.txt");
		int count = 0;
		string readline;
		//imdb_hubs
		/*
		while (getline(in1, readline)) {
			string t = "imdb_" + readline;
			long ind = traceMovie(t, 1);
			
			if (movieSim[ind].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim[ind].size();j++) {
				if (movieSim[ind][j].second > max) {
					max = movieSim[ind][j].second;
					index = movieSim[ind][j].first;
				}
			}
			//cout << max << " ";
			if (max >= 0.5) {
				//cout << "max=" << max << endl;
				matched_pairs.insert(pair<long, long>(ind, index));
				count++;
			}
			
			
			//long index = find_in_truth(ind, 1);
			//if (index != -1) { 
			//	matched_pairs.insert(pair<long, long>(ind, index));
			//	count++;
			//}
			
		}
		cout << "count=" << count << endl;
		*/

		//imdb_isolated
		
		while (getline(in2, readline)) {
			string t = "imdb_" + readline;
			long ind = traceMovie(t, 1);
			if (movieSim[ind].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim[ind].size();j++) {
				if (movieSim[ind][j].second > max) {
					max = movieSim[ind][j].second;
					index = movieSim[ind][j].first;
				}
			}
			//cout << max << " ";
			if (max >= 0.5) {
				//cout << "max=" << max << endl;
				matched_pairs.insert(pair<long, long>(ind, index));
				count++;
			}
		}
		cout << "count=" << count << endl;
		//imdb_community
		while (getline(in5, readline)) {
			string t = "imdb_" + readline;
			long ind = traceMovie(t, 1);
			
			if (movieSim[ind].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim[ind].size();j++) {
				if (movieSim[ind][j].second > max) {
					max = movieSim[ind][j].second;
					index = movieSim[ind][j].first;
				}
			}
			//cout << max << " ";
			if (max >= 0.5) {
				//cout << "max=" << max << endl;
				matched_pairs.insert(pair<long, long>(ind, index));
				count++;
			}
			
			//long index = find_in_truth(ind, 1);
			//if (index != -1) {
			//	matched_pairs.insert(pair<long, long>(ind, index));
			//	count++;
			//}
		}
		cout << "count=" << count << endl;


		//yago_hubs	
		/*
		while (getline(in3, readline)) {
			string t = "yago_" + readline;
			long ind = traceMovie(t, 0);
			
			if (movieSim_yago[ind].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim_yago[ind].size();j++) {
				if (movieSim_yago[ind][j].second > max) {
					max = movieSim_yago[ind][j].second;
					index = movieSim_yago[ind][j].first;
				}
			}
			if (max >= 0.5) {
				matched_pairs.insert(pair<long, long>(index, ind));
				count++;
			}
			
			
			//long index = find_in_truth(ind, 0);
			//if (index != -1) {
			//	matched_pairs.insert(pair<long, long>(index, ind));
			//	count++;
			//}
			
		}
		cout << "count=" << count << endl;
		*/
		//int t;
		//cin>>t;
		//yago_isolated
		
		while (getline(in4, readline)) {
			string t = "yago_" + readline;
			long ind = traceMovie(t, 0);
			if (movieSim_yago[ind].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim_yago[ind].size();j++) {
				if (movieSim_yago[ind][j].second > max) {
					max = movieSim_yago[ind][j].second;
					index = movieSim_yago[ind][j].first;
				}
			}
			//cout << max << " ";
			if (max >= 0.5) {
				//cout << "max=" << max << endl;
				matched_pairs.insert(pair<long, long>(index, ind));
				count++;
			}
		}
		
		cout << "count=" << count << endl;
		//yago_community

		while (getline(in6, readline)) {
			string t = "yago_" + readline;
			long ind = traceMovie(t, 0);
			if (movieSim_yago[ind].size() == 0)continue;
			int max = -1111, index = -1;
			for (int j = 0;j < movieSim_yago[ind].size();j++) {
				if (movieSim_yago[ind][j].second > max) {
					max = movieSim_yago[ind][j].second;
					index = movieSim_yago[ind][j].first;
				}
			}
			//cout << max << " ";
			if (max >= 0.5) {
				//cout << "max=" << max << endl;
				matched_pairs.insert(pair<long, long>(index, ind));
				count++;
			}
			
			//long index = find_in_truth(ind, 0);
			//if (index != -1) {
			//	matched_pairs.insert(pair<long, long>(index, ind));
			//	count++;
			//}
		}

		cout << "count=" << count << endl;
		cout << "The amount of initial matched pairs is " << matched_pairs.size()<< endl;

	}

	void Initialize_priority_queue() {
		cout << "initialize_priority_queue" << endl;
		int count = 0;
		map<long, long>::iterator iter;
		for (iter = matched_pairs.begin();iter != matched_pairs.end();iter++) {
			int index_i = iter->first, index_j = iter->second;
			vector<long> imdb_person, yago_person;
			for (int i = 0;i < imdb_movie_person[index_i].size();i++) {
				imdb_person.push_back(imdb_movie_person[index_i][i].first);
			}
			for (int i = 0;i < yago_movie_person[index_j].size();i++) {
				yago_person.push_back(yago_movie_person[index_j][i].first);
			}

			for (int i = 0;i < imdb_person.size();i++) {
				for (int j = 0;j < yago_person.size();j++) {
					double sim = reCalSim(imdb_person[i], yago_person[j], 0);
					if (sim >0.3) {
						unit_sim unit(imdb_person[i], yago_person[j], sim, 0);
						pairs_queue.push(unit);
						count++;
					}
				}
			}
		}
		cout << "The amount of initial priority queue is " << count << endl;
	}
	void Initialize_relation() {
		cout << "imdb_movie_size:" << imdb_movie_size << endl;
		cout << "imdb_person_size:" << imdb_person_size << endl;
		cout << "yago_movie_size:" << yago_movie_size << endl;
		cout << "yago_person_size:" << yago_person_size << endl;
		for (int i = 0;i < imdb_movie_size;i++) {
			vector<pair<long, int>> t;
			imdb_movie_person.push_back(t);
		}
		for (int i = 0;i < yago_movie_size;i++) {
			vector<pair<long, int>> t;
			yago_movie_person.push_back(t);
		}
		for (int i = 0;i < imdb_person_size;i++) {
			vector<pair<long, int>> t;
			imdb_person_movie.push_back(t);
		}
		for (int i = 0;i < yago_person_size;i++) {
			vector<pair<long, int>> t;
			yago_person_movie.push_back(t);
		}
		ifstream in1, in2;
		in1.open(file7);
		in2.open(file8);
		string readline;
		while (getline(in1, readline)) {
			string s1="", s2="", s3="";
			int index = 0;
			for (;readline[index] != ' ';index++) s1 += readline[index];
			index++;
			for (;readline[index] != ' ';index++)s2 += readline[index];
			index++;
			for (;index < readline.length();index++) s3 += readline[index];
			char c1[100], c2[100], c3[100];
			strcpy(c1, s1.c_str());
			strcpy(c2, s2.c_str());
			strcpy(c3, s3.c_str());
			long n1 = atol(c1), n2 = atol(c2);
			int n3 = atoi(c3);
			imdb_movie_person[n1].push_back(pair<long, int>(n2, n3));
			imdb_person_movie[n2].push_back(pair<long, int>(n1, n3));

		}
		while (getline(in2, readline)) {
			string s1 = "", s2 = "", s3 = "";
			int index = 0;
			for (;readline[index] != ' ';index++) s1 += readline[index];
			index++;
			for (;readline[index] != ' ';index++)s2 += readline[index];
			index++;
			for (;index < readline.length();index++) s3 += readline[index];
			char c1[100], c2[100], c3[100];
			strcpy(c1, s1.c_str());
			strcpy(c2, s2.c_str());
			strcpy(c3, s3.c_str());
			long n1 = atol(c1), n2 = atol(c2);
			int n3 = atoi(c3);
			yago_movie_person[n1].push_back(pair<long, int>(n2, n3));
			yago_person_movie[n2].push_back(pair<long, int>(n1, n3));
		}
		cout << "done" << endl;
	}
	void Iteration() {
		cout << "Iteration start!" << endl;
		int count = 0;
		while (pairs_queue.size()!=0 && pairs_queue.top().sim > 0.46) {
			//cout << "sim="<<pairs_queue.top().sim << endl;
			unit_sim unit = pairs_queue.top();
			pairs_queue.pop();
			long index_i = unit.index_i, index_j = unit.index_j;
			if (unit.movie) {
				cout << "movie" << endl;
				cout << unit.sim << endl;
				cout << matched_pairs.count(index_i) << endl;
				if (matched_pairs.count(index_i) == 0 && unit.sim>=0.48) { 
					cout << "insert!" << endl; 
					matched_pairs.insert(pair<long, long>(index_i, index_j));
				}
				vector<long> imdb_person, yago_person;
				for (int i = 0;i < imdb_movie_person[index_i].size();i++) {
					imdb_person.push_back(imdb_movie_person[index_i][i].first);
				}
				for (int i = 0;i < yago_movie_person[index_j].size();i++) {
					yago_person.push_back(yago_movie_person[index_j][i].first);
				}

				for (int i = 0;i < imdb_person.size();i++) {
					for (int j = 0;j < yago_person.size();j++) {
						double sim = reCalSim(imdb_person[i], yago_person[j], 0);
						if (sim > threshold_high) {
							unit_sim unit(imdb_person[i], yago_person[j], sim, 0);
							pairs_queue.push(unit);
						}
					}
				}
			}
			else {
				count++;
				//cout << "person" << endl;
				vector<long> imdb_movie, yago_movie;
				//if(imdb_person_movie[index_i].size()>=2)cout << imdb_person_movie[index_i].size() << endl;
				for (int i = 0;i < imdb_person_movie[index_i].size();i++) {
					if (matched_pairs.count(imdb_person_movie[index_i][i].first) == 0) { imdb_movie.push_back(imdb_person_movie[index_i][i].first);  }
				}
				for (int i = 0;i < yago_person_movie[index_j].size();i++) {
					yago_movie.push_back(yago_person_movie[index_j][i].first);
				}
				for (int i = 0;i < imdb_movie.size();i++) {
					for (int j = 0;j < yago_movie.size();j++) {
						double sim = reCalSim(imdb_movie[i], yago_movie[j], 1);
						if (sim > threshold_low) {
							//cout << "sim=" << sim << endl;
							unit_sim unit(imdb_movie[i], yago_movie[j], sim, 1);
							pairs_queue.push(unit);
						}
					}
				}
			}
			//cout << "Now there are :"<<matched_pairs.size() << endl;
		}
		cout << matched_pairs.size() << endl;
	}
	void evaluate() {
		string readline;
		int TP = 0, FP = 0,TN=0,FN=0;
		map<long, long>::iterator iter,it;

		map<long, long> FN_pairs;

		for (iter = matched_pairs.begin();iter != matched_pairs.end();iter++) {
			int index_i = iter->first, index_j = iter->second;
			it = truth.find(index_i);
			if (truth.count(index_i) != 0 && it->second == index_j)TP++;
			else if(truth.count(index_i)!=0 && it->second!=index_j)FN_pairs.insert(pair<long, long>(index_i, index_j));
		}

		for (iter = truth.begin();iter != truth.end();iter++) {
			int index_i = iter->first, index_j = iter->second;
			if (matched_pairs.count(index_i) == 0) {
				FN_pairs.insert(pair<long, long>(index_i, index_j));
			}
		}

	

		FP = matched_pairs.size() - TP;
		FN = truth.size() - TP;
		TN = imdb_movie_size*yago_movie_size - TP - FP - FN;

		double prec = 1.0*TP / (TP + FP);
		double acc = 1.0*(TP + TN) / (TP + TN + FN + FP);
		double rec = 1.0*TP / (TP + FN);
		double fm = 2.0*prec*rec / (prec + rec);

		cout << "TP:" << TP << endl;
		cout << "FP:" << FP << endl;
		cout << "TN:" << TN << endl;
		cout << "FN:" << FN <<  endl;

		cout << "prec:" << prec << endl;
		cout << "acc:" << acc << endl;
		cout << "rec:" << rec << endl;
		cout << "fm:" << fm << endl; 

		int count_10 = 0;
		int count_9_10 = 0;
		int count_8_9 = 0;
		int count_7_8 = 0;
		int count_6_7 = 0;
		int count_5_6 = 0;
		int count_5 = 0;
		for (iter = truth.begin();iter != truth.end();iter++) {
			int index_i = iter->first, index_j = iter->second;
			double sim = reCalSim(index_i, index_j, 1);
			if (sim >= 1)count_10++;
			else if (sim >= 0.9)count_9_10++;
			else if (sim >= 0.8)count_8_9++;
			else if (sim >= 0.7)count_7_8++;
			else if (sim >= 0.6)count_6_7++;
			else if (sim >= 0.5)count_5_6++;
			else { 
				checkSim(index_i, index_j, 1);
				cout << "reSim= " << sim << endl;
				count_5++;
			}
		}
		cout << "sim >=1 " << count_10 << endl;
		cout << "sim>=0.9 " << count_9_10 << endl;
		cout << "sim>=0.8 " << count_8_9 << endl;
		cout << "sim>=0.7 " << count_7_8 << endl;
		cout << "sim>=0.6 " << count_6_7 << endl;
		cout << "sim>=0.5 " << count_5_6 << endl;
		cout << "sim<0.5 " << count_5 << endl;
		
		ofstream out1;
		out1.open("FN_pairs.txt");
		map<long, long>::iterator ite;
		for (ite = FN_pairs.begin();ite != FN_pairs.end();ite++) 
			out1<<movieIdList1[ite->first]<<","<<movieIdList2[ite->second]<<","<< checkSim(ite->first, ite->second, 1)<<","<< reCalSim(ite->first, ite->second, 1) <<endl;
		out1.close();
		
	}

	SiGMA(string f1, string f2, string f3, string f4, string f5, string f6,string f7,string f8,string f9,double thresh_l,double thresh_h) {
		file1 = f1;
		file2 = f2;
		file3 = f3;
		file4 = f4;
		file5 = f5;
		file6 = f6;
		file7 = f7;
		file8 = f8;
		file9 = f9;
		threshold_low = thresh_l;
		threshold_high = thresh_h;

		InitializeMovieIdList();
		InitializePersonIdList();
		InitializeSim();
		initializeTruth();
		//Initialize_matched_pair();
		Initialize_matched_pair_from_SCAN();
		Initialize_relation();
		Initialize_priority_queue();
		Iteration();
		evaluate();
		int i = 0;
		cin >> i;
	}
	
};

/*
int main() {
	string f1 = "C:\\Users\\Administrator\\workspace\\backup\\imdb_yago3_golden\\imdb.idlist",
		f2="C:\\Users\\Administrator\\workspace\\backup\\imdb_yago3_golden\\yago.idlist",
		f3="C:\\Users\\Administrator\\workspace\\backup\\imdb_yago3_golden\\imdb_p.idlist",
		f4="C:\\Users\\Administrator\\workspace\\backup\\imdb_yago3_golden\\yago_p.idlist",
		f5="C:\\Users\\Administrator\\workspace\\backup\\imdb_yago3_golden\\imdb_yago_high_fullAKA_clean.txt",
		f6="C:\\Users\\Administrator\\workspace\\backup\\imdb_yago3_golden\\imdb_yago_p_high.txt",
		f7="C:\\Users\\Administrator\\workspace\\cit_rs_v2\\src\\res\\imdb_yago_golden\\p\\Degree\\imdb_movie_person.txt",
		f8="C:\\Users\\Administrator\\workspace\\cit_rs_v2\\src\\res\\imdb_yago_golden\\p\\Degree\\yago_movie_person.txt",
		f9="C:\\Users\\Administrator\\workspace\\cit_rs_v2\\src\\res\\imdb_yago_golden\\p\\Degree\\truth.txt";
	SiGMA(f1,f2,f3,f4,f5,f6,f7,f8,f9,0.25,0.65);
}
*/
