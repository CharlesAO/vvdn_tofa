#ifndef __SHAVE_H__
#define __SHAVE_H__


#define SHAVES_CNT 	(12 - 1)

class Shave{

	private:
		
		static Shave *handles[SHAVES_CNT+1];
		unsigned int id;
		unsigned int busyflag;
		Shave(unsigned int a);
		~Shave(void);

	public:
		static Shave *gethandle(unsigned int a);
		static void destroy(unsigned int a);

		void init(void);
		void deinit(void);
		void reset(void);
		void setdefaultstack(void);
		void start(unsigned int ptr);
		void wait(void);
};

#endif
