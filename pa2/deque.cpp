using namespace std;

template <class T>
Deque<T>::Deque(){
	data = vector<T> (0);
	k1 = 0;
	k2 = -1;
}

template <class T>
void Deque<T>::pushR(T const& newItem)
{
		if (k2 == (int) data.size() - 1)
    	data.push_back(newItem);
    else
    	data[k2 + 1] = newItem;
    k2++;
}

template <class T>
T Deque<T>::popL()
{
    T ret = data[k1];
    k1++;
    if ((int) data.size() >= (k2-k1+1)*2) {
    	data = vector<T> (data.begin() + k1, data.begin() + k2 + 1);
    	k2 -= k1;
    	k1 = 0;
    }
    return ret;
}

template <class T>
T Deque<T>::popR()
{
    T ret = data[k2];
    k2--;
   if ((int) data.size() >= (k2-k1+1)*2) {
    	data = vector<T> (data.begin() + k1, data.begin() + k2 + 1);
    	k2 -= k1;
    	k1 = 0;
    }
    return ret;
}

template <class T>
T Deque<T>::peekL()
{
    return data[k1];
}

template <class T>
T Deque<T>::peekR()
{
    return data[k2];
}

template <class T>
bool Deque<T>::isEmpty() const
{
    return k2 + 1 == k1;
}
