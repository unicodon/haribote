//////////////////////////////////////////////////////////////////
//
// MySTL:軽量かつ必要最低限のSTL
//   ソートとかハッシュ関数とかは実装しませぬ
//
//////////////////////////////////////////////////////////////////

#ifndef MYSTL_H
#define MYSTL_H

#define INIT_SIZE 50

/////////////////////////////////////////
//stack
template <class T>
class MyStack{
private:
	T* buf;
	int current;
	int reserve_size;

public:
	MyStack();
	MyStack(const MyStack<T>& other);
	virtual ~MyStack();

	MyStack<T>& operator = (const MyStack<T>& other); 

	int size();
	void pop();
	T top();
	void push(const T& val);
};


template <class T> 
MyStack<T>::MyStack(){
	current = -1;
	buf = new T[INIT_SIZE];
	reserve_size = INIT_SIZE;
}

template <class T> 
MyStack<T>::MyStack(const MyStack<T>& other){
	current = other.current;
	reserve_size = ohter.reserve.size;

	buf = new T[reserve_size];
	memcpy(buf,other.buf,sizeof(T)*reserve_size);
}


template <class T> 
MyStack<T>::~MyStack(){
	delete[] buf;
}

template <class T> 
MyStack<T>& MyStack<T>::operator = (const MyStack<T>& other){
	current = other.current;
	reserve_size = ohter.reserve.size;

	buf = new T[reserve_size];
	memcpy(buf,other.buf,sizeof(T)*reserve_size);

	return *this;
}


template <class T> 
void MyStack<T>::pop(){
	if(current >= 0){
		current--;
	}
}

template <class T> 
T MyStack<T>::top(){
	if(current >= 0){
		return *(buf + current);
	}else{
		T t;
		return t;
	}
}

template <class T> 
void MyStack<T>::push(const T& val){
	current++;
	if(current >= reserve_size){
		reserve_size *= 2;
		T* temp = new T[reserve_size];
		memcpy(temp,buf,sizeof(T)*current);
		delete[] buf;
		buf = temp;
	}

	buf[current] = val;

}

template <class T> 
int MyStack<T>::size(){
	return current+1;
}


/*
/////////////////////////////////////////
//vector
template <class T>
class MyVector{
private:
	T* buf;
	int current;
	int reserve_size;

public:
	MyVector();
	MyVector(const MyVector<T> & other);
	virtual ~MyVector();

	MyVector<T> &operator=(const MyVector<T>& other);

	int size();
	void clear();
	T& operator [](int n);
	T back();
	T front();
	void push_back(const T& val);
	void pop_back();
};

template <class T> 
MyVector<T>::MyVector(){
	current = -1;
	buf = new T[INIT_SIZE];
	reserve_size = INIT_SIZE;
}

template <class T> 
MyVector<T>::MyVector(const MyVector<T>& other){
	current = other.current;
	reserve_size = ohter.reserve.size;

	buf = new T[reserve_size];
	memcpy(buf,other.buf,sizeof(T)*reserve_size);
}

template <class T> 
MyVector<T>::~MyVector(){
	delete[] buf;
}

template <class T> 
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other){
	if(this == &other)return *this;

	current = other.current;
	reserve_size = other.reserve_size;

	delete[] buf;
	buf = new T[reserve_size];
	memcpy(buf,other.buf,sizeof(T)*reserve_size);

	return *this;
}


template <class T> 
void MyVector<T>::clear(){
	current = -1;
}


template <class T> 
void MyVector<T>::pop_back(){
	if(current >= 0){
		current--;
	}
}

template <class T> 
T MyVector<T>::back(){
	if(current >= 0){
		return *(buf + current);
	}else{
		T t;
		return t;
	}
}

template <class T> 
T MyVector<T>::front(){
	if(current >= 0){
		return *(buf);
	}else{
		T t;
		return t;
	}
}


template <class T> 
void MyVector<T>::push_back(const T& val){
	current++;
	if(current >= reserve_size){
		reserve_size *= 2;
		T* temp = new T[reserve_size];
		memcpy(temp,buf,sizeof(T)*current);
		delete[] buf;
		buf = temp;
	}

	buf[current] = val;

}

template <class T> 
int MyVector<T>::size(){
	return current+1;
}

template <class T> 
T& MyVector<T>::operator [](int n){
	while(n > reserve_size){
		reserve_size *= 2;
		T* temp = new T[reserve_size];
		memcpy(temp,buf,sizeof(T)*(current+1));
		delete[] buf;
		buf = temp;
	}
	
	return *(buf+n);

}
*/

#endif
