#include "DBMapFill.h"


//------------------------------------------------------------------------------
DBMapFill::DBMapFill()
{
	mpctrl = NULL;
}

//------------------------------------------------------------------------------
DBMapFill::~DBMapFill()
{
	mdb.desconectar();
}

//------------------------------------------------------------------------------
void DBMapFill::init(Control* pctrl)
{
	
	mpctrl = pctrl;
	mprocessed = 0;
	mprocessed_old = 0;

	if (mdb.conectar(SIM_DB_HOST, SIM_DB_PORT, mpctrl->mdb_name, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) { return; }
}

//------------------------------------------------------------------------------
void DBMapFill::execute()
{
	string line, substr;
	vector<Cadena> vnode, vnodetag, vway, vwaytag, vwaynode, vrel, vreltag, vrelmember, vtime;
	vector<Cadena>::const_iterator it;
	Cadena linec, cad;
	const char* pc = NULL;
	Cadena cadquery;
	Cadena file;
	int timestp;
	long long int nfirst=0, nlast=0;
	long long int nid, wid, rid;
	int version;
	double lat, lon;

	bool node_in  = false;
	bool node_out = false;
	bool node_tag = false;
	bool way_in   = false;
	bool way_out  = false;
	bool way_tag  = false;
	bool way_node = false;
	bool rel_in = false;
	bool rel_out = false;
	bool rel_tag = false;
	bool rel_member = false;

	int line_long, line_long_trim, count_nodes = 0, count_ways = 0, count_rels = 0;
	int count_node_tags = 0;
	int count_way_tags = 0;
	int count_way_nodes = 0;
	int count_rel_tags = 0;
	int count_rel_members = 0;

	ifstream f(mpctrl->mfilemap.getCadena());
	if (!f.is_open())
	{
		cout << "Error opening file:[" << mpctrl->mfilemap.getCadena() << "]" << endl;
		return;
	}

	cout << mpctrl->mfseek << endl;

	f.seekg(mpctrl->mfseek, ios::beg);

	//20 185 540 000 Total
	//13920000000 just before Ways
	//20005540000 Relations

	while (getline(f, line))
	{
		line_long = linec.getLong();
		mprocessed = mprocessed + line_long;
		linec = line.c_str();
		linec.lTrim();
		linec.rTrim();
		line_long_trim = linec.getLong();
		pc = linec.getCadena();

		if (*pc == '<')
		{


			/*
			------------------------------------------------------------------------------------------------------------------------------------------------------------------
													   NODES & TAGS
			------------------------------------------------------------------------------------------------------------------------------------------------------------------
			<node id="13828895" version="2" timestamp="2019-11-18T10:44:54Z" lat="37.4142656" lon="-2.9190574"/>
			<node id="13828899" version="2" timestamp="2011-02-21T22:18:22Z" lat="37.5252917" lon="-2.6827983">
				<tag k="created_by" v="JOSM"/>
			</node>
			*/
	
			if ((*(pc + 1) == 'n') && (*(pc + 2) == 'o'))
			{
				node_in = true;
				node_out = false;
				node_tag = false;
				count_node_tags = 0;

				stringstream ss(linec.getCadena());
				vnode.clear();
				vnodetag.clear();
				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str();
					cad.rTrim();
					cad.lTrim();
					vnode.push_back(cad);
				}

				if ((*(pc + line_long_trim - 2) == '/') && (*(pc + line_long_trim - 1) == '>'))
				{
					node_in = false;
					node_out = true;
					count_nodes++;
				}
			}
			else if (((*(pc + 1) == 't') && (*(pc + 2) == 'a')) && node_in)
			{
				node_tag = true;
				count_node_tags++;
				stringstream ss(linec.getCadena());
				
				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str();
					cad.rTrim();
					cad.lTrim();
					vnodetag.push_back(cad);
				}
			}
			else if (((*(pc + 1) == '/') && (*(pc + 2) == 'n')) && node_in)
			{

				count_nodes++;
				node_in = false;
				node_out = true;
			}

			/*
			------------------------------------------------------------------------------------------------------------------------------------------------------------------
													   WAY & TAGS
			------------------------------------------------------------------------------------------------------------------------------------------------------------------
			<way id="42386059" version="4" timestamp="2018-03-29T19:45:42Z" changeset="57643956" uid="581863" user="pedrobv">
			   <nd ref="4519406632"/>
			   <nd ref="309993539"/>
			   <nd ref="309993569"/>
			   <tag k="highway" v="tertiary"/>
			   <tag k="lanes" v="2"/>
			   <tag k="ref" v="GIV-5341"/>
			</way>
			*/

			else if ((*(pc + 1) == 'w') && (*(pc + 2) == 'a'))
			{
				way_in = true;
				way_out = false;
				way_tag = false;
				way_node = false;
				count_way_tags = 0;
				count_way_nodes = 0;

				stringstream ss(linec.getCadena());
				vway.clear();
				vwaytag.clear();
				vwaynode.clear();

				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str();cad.rTrim();cad.lTrim();
					vway.push_back(cad);
				}

				if ((*(pc + line_long_trim - 2) == '/') && (*(pc + line_long_trim - 1) == '>'))
				{
					way_in = false;
					way_out = true;
					count_ways++;
				}
			}
			else if (((*(pc + 1) == 't') && (*(pc + 2) == 'a')) && way_in)
			{
				way_tag = true;
				count_way_tags++;
				stringstream ss(linec.getCadena());

				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str();cad.rTrim();cad.lTrim();
					vwaytag.push_back(cad);
				}
			}
			else if (((*(pc + 1) == 'n') && (*(pc + 2) == 'd')) && way_in)
			{
				way_node = true;
				count_way_nodes++;
				stringstream ss(linec.getCadena());
				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str();cad.rTrim();cad.lTrim();
					vwaynode.push_back(cad);
				}
			}
			else if (((*(pc + 1) == '/') && (*(pc + 2) == 'w')) && way_in)
			{
				count_ways++;
				way_in = false;
				way_out = true;
			}

			/*
			--------------------------------------------------------------------------------------------------------------------------
													   RELATIONS
			--------------------------------------------------------------------------------------------------------------------------
			<relation id="8643" version="586" timestamp="2020-02-13T04:18:31Z" changeset="80933065" uid="90780" user="Verdy_p">
		 		<member type="node" ref="1251610256" role="label"/>
				<member type="node" ref="65442261" role="admin_centre"/>
				<member type="way" ref="50344406" role="outer"/>
				<tag k="name:lt" v="Ryt Pirnai"/>
				<tag k="type" v="boundary"/>
				<tag k="wikidata" v="Q12709"/>
			</relation>
			--------------------------------------------------------------------------------------------------------------------------
			*/
			else if ((*(pc + 1) == 'r') && (*(pc + 2) == 'e'))
			{
				rel_in = true;
				rel_out = false;
				rel_tag = false;
				rel_member = false;
				count_rel_tags = 0;
				count_rel_members = 0;

				stringstream ss(linec.getCadena());
				vrel.clear();
				vreltag.clear();
				vrelmember.clear();

				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str(); cad.rTrim(); cad.lTrim();
					vrel.push_back(cad);
				}

				if ((*(pc + line_long_trim - 2) == '/') && (*(pc + line_long_trim - 1) == '>'))
				{
					rel_in = false;
					rel_out = true;
					count_rels++;
				}

			}
			else if (((*(pc + 1) == 't') && (*(pc + 2) == 'a')) && rel_in)
			{
				rel_tag = true;
				count_rel_tags++;
				stringstream ss(linec.getCadena());

				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str(); cad.rTrim(); cad.lTrim();
					vreltag.push_back(cad);
				}
			}
			else if (((*(pc + 1) == 'm') && (*(pc + 2) == 'e')) && rel_in)
			{
				rel_member = true;
				count_rel_members++;
				stringstream ss(linec.getCadena());

				while (ss.good())
				{
					getline(ss, substr, '"');
					cad = substr.c_str(); cad.rTrim(); cad.lTrim();
					vrelmember.push_back(cad);
				}
			}
			else if (((*(pc + 1) == '/') && (*(pc + 2) == 'r')) && rel_in)
			{
				count_rels++;
				rel_in = false;
				rel_out = true;
			}

		} //end line


		//---------------------------------------------------------------------------------------------------------------------------------------
		//                         PROCESSING
		//---------------------------------------------------------------------------------------------------------------------------------------¡
		if (node_out)
		{
			nid = atoll(vnode[1].getCadena());
			version = atoi(vnode[3].getCadena());

			lat = atof(vnode[7].getCadena());
			lon = atof(vnode[9].getCadena());

			timestp = get_timestamp_num(vnode[5]);
			
			cadquery.formCadena("INSERT INTO Node_pos(nid, ver, timestmp, lon, lat, nways) VALUES (%lld, %d, %ld, %lf, %lf, 0)", nid, version, timestp, lon, lat);
			//if (!mdb.ejecutarConsulta(cadquery)) {cout << "error: Node_pos" << endl;}
			if (!mdb.ejecutarConsulta(cadquery)) 
			{ 
				cout << "-np"; 
			}
			else
			{
				
				int count = 0;
				for (int i = 0; i < count_node_tags; i++)
				{
					if (node_tag)
					{
						if (vnodetag[count + 1].getLong() >= 200) vnodetag[count + 1].borraCaracter(199, '\0');
						if (vnodetag[count + 3].getLong() >= 200) vnodetag[count + 3].borraCaracter(199, '\0');
						cadquery.formCadena("INSERT INTO Node_tags(nid, ver, timestmp, key_tag, value) VALUES (%lld, %d, %ld, '%s', '%s')", nid, version, timestp, vnodetag[count + 1].getCadena(), vnodetag[count + 3].getCadena());
						//if (!mdb.ejecutarConsulta(cadquery)) {cout << "error: Node_tags" << endl;}
						if (!mdb.ejecutarConsulta(cadquery)) { cout << "-nt"; }
					}
					count = count + 5;
				}
				
			}

			node_in = false;
			node_out = false;
			node_tag = false;
			count_node_tags = 0;

		}
		else if (way_out)
		{
			wid = atoll(vway[1].getCadena());
			version = atoi(vway[3].getCadena());
			timestp = get_timestamp_num(vway[5]);
			cadquery.formCadena("INSERT INTO Ways(wid, ver, timestmp, nfirst, nlast) VALUES(%lld, %d, %d, %lld, %lld)", wid, version, timestp, atoll(vwaynode[1].getCadena()), atoll(vwaynode[count_way_nodes*3-2].getCadena()));
			//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Ways" << endl; }
			if (!mdb.ejecutarConsulta(cadquery)) 
			{ 
				cout << "-w"; 
			}
			else
			{
				int count = 0;
				for (int i = 0; i < count_way_tags; i++)
				{
					if (way_tag)
					{
						if (vwaytag[count + 1].getLong() >= 200) vwaytag[count + 1].borraCaracter(199, '\0');
						if (vwaytag[count + 3].getLong() >= 200) vwaytag[count + 3].borraCaracter(199, '\0');

						cadquery.formCadena("INSERT INTO Way_tags (wid, ver, timestmp, key_tag, value) VALUES (%lld, %d, %d, '%s', '%s')", wid, version, timestp, vwaytag[count + 1].getCadena(), vwaytag[count + 3].getCadena());
						//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Way_tags" << endl; }
						if (!mdb.ejecutarConsulta(cadquery)) { cout << "-wt"; }
					}
					count = count + 5;

				}

				count = 0;
				int count_nodes = 0;
				for (int i = 0; i < count_way_nodes; i++)
				{
					if (way_node)
					{
						nid = atoll(vwaynode[count + 1].getCadena());
						cadquery.formCadena("INSERT INTO Way_nodes(wid, ver, timestmp, norder, nid) VALUES(%lld, %d, %d, %d, %lld)", wid, version, timestp, count_nodes, nid);
						//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Way_nodes" << endl; }
						if (!mdb.ejecutarConsulta(cadquery)) { cout << "-wn"; }

						cadquery.formCadena("INSERT INTO Node_ways(nid, wid, w_ver, w_norder) VALUES(%lld, %lld, %d, %d)", nid, wid, version, count_nodes);
						//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Node_ways" << endl; }
						if (!mdb.ejecutarConsulta(cadquery)) { cout << "-nw"; }
					}

					count = count + 3;
					count_nodes++;
				}
			}

			way_in  = false;
			way_out = false;
			way_tag = false;
			way_node = false;
			count_way_tags = 0;
			count_way_nodes = 0;
		}
		else if (rel_out)
		{
			rid = atoll(vrel[1].getCadena());
			version = atoi(vrel[3].getCadena());
			timestp = get_timestamp_num(vrel[5]);

			cadquery.formCadena("INSERT INTO Relations(rid, ver, timestmp) VALUES(%lld, %d, %d)", rid, version, timestp);
			//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Relations" << endl; }
			if (!mdb.ejecutarConsulta(cadquery)) 
			{ 
				cout << "-r"; 
			}
			else
			{

				int count = 0;
				for (int i = 0; i < count_rel_tags; i++)
				{
					if (rel_tag)
					{
						if (vreltag[count + 1].getLong() >= 200) vreltag[count + 1].borraCaracter(199, '\0');
						if (vreltag[count + 3].getLong() >= 200) vreltag[count + 3].borraCaracter(199, '\0');
						cadquery.formCadena("INSERT INTO Relation_tags(rid, ver, timestmp,  key_tag, value) VALUES(%lld, %d, %d, '%s', '%s')", rid, version, timestp, vreltag[count + 1].getCadena(), vreltag[count + 3].getCadena());
						//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Relation_tags" << endl; }
						if (!mdb.ejecutarConsulta(cadquery)) { cout << "-rt"; }
					}
					count = count + 5;
				}

				count = 0;
				int count_ref = 0;
				long long int ref;

				for (int i = 0; i < count_rel_members; i++)
				{
					if (rel_member)
					{
						ref = atoll(vrelmember[count + 3].getCadena());
						cadquery.formCadena("INSERT INTO Relation_members(rid, ver, timestmp, ref, norder, type, role) VALUES(%lld, %d, %d,%lld, %d, '%s' ,'%s' )", rid, version, timestp, ref, count_ref, vrelmember[count + 1].getCadena(), vrelmember[count + 5].getCadena());
						//if (!mdb.ejecutarConsulta(cadquery)) { cout << "error: Relation_members" << endl; }
						if (!mdb.ejecutarConsulta(cadquery)) { cout << "-rm"; }
						count_ref++;
					}
					count = count + 7;
				}
			}

			rel_in = false;
			rel_out = false;
			rel_tag = false;
			rel_member = false;
			count_rel_tags = 0;
			count_rel_members = 0;
		}

		if (mprocessed > mprocessed_old + 1000000)
		{
			cout <<  "  mprocessed: " << mprocessed << endl;
			cout <<  "      nodes: " << count_nodes << endl;
			cout << "       ways:  " << count_ways << endl;
			cout << "       rels:  " << count_rels << endl;
			mprocessed_old = mprocessed;
		}

		if (mpctrl->mbfseek == true)
		{
			if (mprocessed > 1000000000 + 20000)
			{
				cout << "  Final bytes processed:[" << mprocessed << "]" << endl;
				f.close();
				return;
			}
		}
	}

	f.close();

}

//------------------------------------------------------------------------------
int DBMapFill::get_timestamp_num(Cadena& cadtimestamp)
{
	vector<Cadena> vtime;
	string line, substr;
	int timestp;

	cadtimestamp.rTrim();
	cadtimestamp.borraCaracter(11, ' ');
	cadtimestamp.borraCaracter(20, '\0');
	cadtimestamp.ReemplazarCadena("-", ":");
	cadtimestamp.ReemplazarCadena("-", ":");
	cadtimestamp.ReemplazarCadena(" ", ":");
	stringstream ss(cadtimestamp.getCadena());
	vtime.clear();

	while (ss.good()) { getline(ss, substr, ':'); vtime.push_back(substr.c_str()); }
	cadtimestamp = vtime[0] + vtime[1] + vtime[2];// +vtime[3] + vtime[4] + vtime[5];
	timestp = cadtimestamp.toUDWordType();
	return timestp;
}

