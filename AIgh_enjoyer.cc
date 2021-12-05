#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME gh_enjoyer


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  typedef vector<char> VAR;
  typedef pair<int,int> PI;
  typedef vector<bool> VOOL;
  typedef vector<int> VINT;
  
  typedef vector<VAR> map;
  typedef queue<PI> cua;
  typedef vector<PI> VAIR;
  typedef vector<VAIR> map_coord;
  typedef vector<VOOL> map_vis;

  VAIR dirs = { {0,1}, {0,-1}, {1,0}, {-1,0} };

  map scan() {

    map my_map (70,vector<char>(70));

    for(int i = 0; i < 70; ++i){
      for(int j = 0; j < 70; ++j){

        Cell c = cell(i,j);

        if(c.type == WALL) my_map[i][j] = 'w';
        else if(c.unit_id != -1){
          if(unit(c.unit_id).player == me()) my_map[i][j] = 'w';
          else my_map[i][j] = 't';
        }
        else if(c.type == GRASS) my_map[i][j] = 'g';
        else if(c.type == CITY){
          if(city_owner(c.city_id) == me()) my_map[i][j] = 'g';
          else my_map[i][j] = 't';
        } 
        else if(c.type == PATH){
          if(path_owner(c.path_id) == me()) my_map[i][j] = 'g';
          else my_map[i][j] = 't';
        }  
      }
    }
    return my_map;
  }

  bool existeix_vei (const map &M, int i, int j) {
  return i >= 0 and j >= 0 and i < 70 and j < 70 and M[i][j] != 'w';
  }

// Retorna el cami de orig cap a dest
  VAIR calcula_cami(const map_coord &prev, const PI &orig, const PI &dest) {
    stack<pair<int,int>> S;
    PI actual = dest;
    while (actual != orig) {
      S.push(actual);
      actual = prev[actual.first][actual.second];
    }

    VAIR resultat;
    while (not S.empty()) {
      resultat.push_back(S.top());
      S.pop();
    }

    return resultat;

  }

  VAIR path_calculator (map &M, cua &Q, map_vis &dist, map_coord &prev, PI orig){
    pair<int,int> p = Q.front();
    int v_i = p.first;
    int v_j = p.second;
    Q.pop();

    for (auto& d : dirs) {
      int n_i = v_i + d.first;
      int n_j = v_j + d.second;
      if (existeix_vei(M,n_i,n_j) and dist[n_i][n_j] == false){
          dist[n_i][n_j] = true;
          prev[n_i][n_j] = {v_i,v_j};
          Q.push({n_i,n_j});

          if (M[n_i][n_j] == 't') {
              VAIR cami = calcula_cami(prev,orig,{n_i,n_j});
              return cami;
          }
        }
      }

    return path_calculator(M,Q,dist,prev,orig);
  }

  Dir convert_to_dir(PI coord, PI orig){
    coord.first -= orig.first;
    coord.second -= orig.second;
    for(int i = 0; i < 3; ++i){
      if(dirs[i] == coord){
        if(i == 0) return RIGHT;
        else if(i == 1) return LEFT;
        else if(i == 2) return BOTTOM;
      } 
    }
    return TOP;
  }

  void conquer(){

    map my_map = scan();
    VINT units = my_units(me());

    for(auto& id: units){

      Unit u = unit(id);
      Pos actual = u.pos;

      PI orig = {actual.i,actual.j};
      cua Q; Q.push(orig);
      map_vis my_map_vis(70,VOOL((70),false));
      map_coord my_map_coord(70,VAIR(70));
      my_map_vis[orig.first][orig.second] = true;

      VAIR p = path_calculator(my_map,Q,my_map_vis,my_map_coord,orig);
      move(id,convert_to_dir(p[0],orig));
    }
  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    conquer();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
