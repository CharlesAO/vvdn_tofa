
#ifndef _SHAPE_H_
#define _SHAPE_H_

class Shape{
	private:
		int type;

   	public:
      	Shape();
      	~Shape(); 
   
      	void setType(int type);
      	int getType(void);
      	void printme(void);
  
};
#endif
