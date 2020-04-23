#include"TypeTable.hh"
#include<string>
#include<iostream>


//Сделать конструкторы выделяющие динамическую память
//
//
//
struct ISyntaxTreeNode {
  NodeTreeTypes::Types type; 

  virtual void SetRight(ISyntaxTreeNode * right) = 0;
  virtual void SetLeft(ISyntaxTreeNode * left) = 0;

  virtual ISyntaxTreeNode * GetRight() const = 0;
  virtual ISyntaxTreeNode * GetLeft() const = 0;

  virtual void SetData(int data_) = 0;
  virtual int GetData() const = 0;
  
  virtual std::string GetName() const = 0;
  virtual void SetName(std::string name_) = 0;

  void AbortAndMessage() const {
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };

  virtual ~ISyntaxTreeNode() {};
};

struct SyntaxNodeStream final : public ISyntaxTreeNode {

//Block of unused functions
//------------------------------------------------------------------------------------
  void SetData(int data_) override{
    AbortAndMessage();
  };
  int GetData() const override{
    AbortAndMessage();
    return 0;
  };
  void SetName(std::string name_) override{
    AbortAndMessage();
  };
  std::string GetName() const override{
    AbortAndMessage();
    return "ERROR";
  };
  void SetRight(ISyntaxTreeNode * right) override{
    AbortAndMessage();
  };
  ISyntaxTreeNode * GetRight() const override{
    return NULL;
  };
  void SetLeft(ISyntaxTreeNode * left) override{
    AbortAndMessage();
  };

  ISyntaxTreeNode * GetLeft() const override{
    return NULL;
  };
//------------------------------------------------------------------------------------
};

struct SyntaxNodeNum : public ISyntaxTreeNode {
  int data;

  void SetData(int data_) override{
    data = data_;
  };

  int GetData() const override{
    return data;
  };

  virtual ~SyntaxNodeNum() {};

//Block of unused functions
//------------------------------------------------------------------------------------
  void SetName(std::string name_) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };
  std::string GetName() const override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
    return "ERROR";
  };
  void SetRight(ISyntaxTreeNode * right) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };
  ISyntaxTreeNode * GetRight() const override{
    return NULL;
  };
  void SetLeft(ISyntaxTreeNode * left) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };

  ISyntaxTreeNode * GetLeft() const override{
    return NULL;
  };
//------------------------------------------------------------------------------------

}; 

struct SyntaxNodeStr : public ISyntaxTreeNode {
  std::string name;

  std::string GetName() const override{
    return name;
  };

  void SetName(std::string name_) override{
    name = name_;
  };

  virtual ~SyntaxNodeStr() {};

//Block of unused functions
//------------------------------------------------------------------------------------
  void SetData(int data_) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };
  int GetData() const override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
    return 0;
  };
  void SetRight(ISyntaxTreeNode * right) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };
  ISyntaxTreeNode * GetRight() const override{
    return NULL;
  };
  void SetLeft(ISyntaxTreeNode * left) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };

  ISyntaxTreeNode * GetLeft() const override{
    return NULL;
  };
//------------------------------------------------------------------------------------
};

struct SyntaxNodeUno : public ISyntaxTreeNode {
  ISyntaxTreeNode * lft;

  void SetLeft(ISyntaxTreeNode * left) override{
    lft = left;
  };

  ISyntaxTreeNode * GetLeft() const override{
    return lft;
  };

  virtual ~SyntaxNodeUno() {};

//Block of unused functions
//------------------------------------------------------------------------------------
  void SetData(int data_) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };
  void SetName(std::string name_) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };
  std::string GetName() const override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
    return "ERROR";
  };
  int GetData() const override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
    return 0;
  };

  void SetRight(ISyntaxTreeNode * right) override{
    std::cout << "You mustn't use this function in this node" << std::endl;
    std::abort();
  };

  ISyntaxTreeNode * GetRight() const override{
    return NULL;
  };
//------------------------------------------------------------------------------------
};

struct SyntaxNodeBin final : public SyntaxNodeUno {
  ISyntaxTreeNode * rhs;

  void SetRight(ISyntaxTreeNode * right) override{
    rhs = right;
  };

  ISyntaxTreeNode * GetRight() const override {
    return rhs;
  };
};
