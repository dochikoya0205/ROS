# Study_docx//
// Created by user on 2021-06-07.
//

#ifndef VEC_VEC_H
#define VEC_VEC_H
#include <memory>

template <typename T>
class Vec{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T& reference_type;
    typedef const T& const_reference_type;

    Vec();
    explicit Vec(size_type size, const T& val = T()/*예를 들어 int형이라면 int 0이 들어갈 것이다.*/);
    Vec(const Vec<T>& rhs);
    ~Vec();
    Vec& operator=(const Vec<T>& rhs);

    size_type size() const;
    T& operator[](int index);
    const T& operator[](int index) const;

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    void push_back(const T& val);

private:
    T* data;
    T* avail;
    T* limit;

    std::allocator<T> alloc; //allocator는 생성자없이 공간만 할당한다.

    void create();
    void create(size_type n, const T& val);
    void create(const_iterator b, const_iterator e);
    void uncreate();

    void grow();
    void unchecked_append(const T& val);

};

template <typename T>
void Vec<T>::create()
{
    this->data = this->avail = this->limit = 0;
}

template <typename T>
void Vec<T>::create(size_type n, const T& val) {
    this->data = this->alloc.allocate(n);
    std::uninitialized_fill(this->data, this->data + n, val); //data부터 data+n까지 val을 채워준다.
    this->avail = this->data + n;
    this->limit = this->data + n;
}

template <typename T>
void Vec<T>::create(Vec<T>::const_iterator b, Vec<T>::const_iterator e) {
    this->data = this->alloc.allocate(e - b);
    std::uninitialized_copy(b, e, this->data); //b부터 e까지 this->data를 복사한다.

    this->avail = this->data + (e - b); //이터레이터끼리 차이를 먼저 구하고 그 다음에 데이터에 더한다. 만일 소괄호를 쓰지 않으면 오른쪽부터 계산하기에 오류가 뜬다.
    this->limit = this->data + (e - b);
    //데이터가 꽉 찬 상태에서는 avail과 limit가 동일하다.
}

template <typename T>
void Vec<T>::uncreate() {
   if(this->data ) {
       typename Vec<T>::iterator it = avail; //it는 데이터가 들어 잇는 다음 위치를 가리킨다.
       while (it != data) {
           this->alloc.destroy(it);
           --it;
       }
       this->alloc.deallocate(this->data, this->limit - this->data); //공간을 해제한다.
   }
   this->data = this->avail = this->limit = 0;
}

//공간을 확대하는 함수:grow()
template <typename T>
void Vec<T>::grow(){
    typename Vec<T>::size_type new_size;

    if(this->data ){
        new_size = 1; //아무것도 없는 공간에 새로운 데이터를 넣는다.
    } else {
        new_size = 2 * (this->limit - this->data); //공간을 2배로 늘여 공간을 확보한다.
    }
    Vec<T>::iterator new_data = this->alloc.allocate(new_size);
    Vec<T>::iterator new_avail = std::uninitialized_copy(this->data, this->avail, new_data);
    this->uncreate(); //원본을 날린다.
    //포인터 값을 새로 조정한다.
    this->data = new_data;
    this->avail = new_avail;
    this->limit = this->data + new_size;
}

template <typename T>
void Vec<T>::unchecked_append(const T &val) {
    alloc.construct(this->avail, val);
    ++this->avail;
}

template <typename T>
Vec<T>::Vec(){
    this->create();
}

template <typename T>
Vec<T>::Vec(size_t size, const T& val){
    this->create(size, val);
}

template <typename T>
Vec<T>::Vec(const Vec<T>& rhs){
    this->create(rhs.cbegin(), rhs.cend());
}

template <typename T>
Vec<T>::~Vec(){
    this->uncreate();
}

template<typename T>
Vec<T>& Vec<T>::operator=(const Vec<T> &rhs) {
    if(this != &rhs){
        this->uncreate();

        this->create(rhs.cbegin(), rhs.cend());
    }
    return *this;
}

template <typename T>
typename Vec<T>::size_type /*<<주의가 필요한 내용*/ Vec<T>::size() const{
    return (this->avail - this->data); //포인터의 떨어진 차이만큼을 반환한다.
}

template <typename T>
T& Vec<T>::operator[](int index) {
    return this->data[index];
}

template <typename T>
const T& Vec<T>::operator[](int index) const{
    return this->data[index];
}

template <typename T>
typename Vec<T>::iterator Vec<T>::begin(){
    return this->data;
}

template <typename T>
typename Vec<T>::iterator Vec<T>::end(){
    return this->avail;
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::cbegin() const{
    return this->data;
}

template <typename T>
typename Vec<T>::const_iterator Vec<T>::cend() const{
    return this->avail;
}

template <typename T>
void Vec<T>::push_back(const T &val) {
    if(this->avail == this->limit)
        this->grow();
    this->unchecked_append(val);
}

#endif //VEC_VEC_H
