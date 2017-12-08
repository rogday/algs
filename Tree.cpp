#include "Tree.h"

void Tree::insert(int x) {
  if (!val) {
    val = new int(x);
  } else {
    if (x > *val) {
      if (!R)
        R = new Tree;
      R->insert(x);
    } else {
      if (!L)
        L = new Tree;
      L->insert(x);
    }
  }
};

bool Tree::search(int x) {
  if (val) {
    if (*val == x)
      return true;
    if (x > *val) {
      if (!R)
        return false;
      return R->search(x);
    } else {
      if (!L)
        return false;
      return L->search(x);
    }
  }
  return false;
}

Tree::~Tree() {
  delete L;
  delete val;
  delete R;
}

std::ostream &operator<<(std::ostream &strm, Tree &tree) {
  if (tree.val) {
    if (tree.L)
      strm << *(tree.L);
    strm << *(tree.val) << ' ';
    if (tree.R)
      strm << *(tree.R);
  }
  return strm;
}

void Tree::saveToFile(std::string filename) {
  if (!val)
    return;

  std::fstream file;
  file.open(filename, std::fstream::out | std::fstream::binary);

  std::deque<Tree *> stk1;
  std::deque<Tree *> stk2;

  int pos = 0;
  const int nul = -1;

  stk1.push_front(this);

  while (!stk1.empty()) {
    Tree *ptree = stk1.back();
    stk1.pop_back();

    file.write(reinterpret_cast<const char *>(ptree->val),
               sizeof *(ptree->val));

    if (ptree->L) {
      ++pos;
      file.write(reinterpret_cast<const char *>(&pos), sizeof pos);
      stk2.push_front(ptree->L);
    } else
      file.write(reinterpret_cast<const char *>(&nul), sizeof nul);

    if (ptree->R) {
      ++pos;
      file.write(reinterpret_cast<const char *>(&pos), sizeof pos);
      stk2.push_front(ptree->R);
    } else
      file.write(reinterpret_cast<const char *>(&nul), sizeof nul);

    if (stk1.empty())
      stk1.swap(stk2);
  }

  file.close();
};

bool Tree::searchInFile(std::string filename, int value) {
  std::fstream file;
  file.open(filename, std::fstream::in | std::fstream::binary);

  int pos = 0;
  bool flag = false;

  file.seekg(0, file.end);
  int length = file.tellg();
  file.seekg(0, file.beg);

  int val, sk = 0;

  if (length != 0)
    while (sk != -1) {
      file.read(reinterpret_cast<char *>(&val), sizeof val);

      if (val == value) {
        flag = true;
        break;
      }

      file.read(reinterpret_cast<char *>(&sk), sizeof sk);

      if (value > val)
        file.read(reinterpret_cast<char *>(&sk), sizeof sk);

      if (sk != -1)
        file.seekg(3 * sk * (sizeof sk));
    }

  file.close();

  return flag;
};
