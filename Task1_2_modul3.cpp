/*Ðåàëèçóéòå ñòðóêòóðó äàííûõ òèïà “ìíîæåñòâî ñòðîê” íà îñíîâå äèíàìè÷åñêîé
õåø-òàáëèöû ñ îòêðûòîé àäðåñàöèåé. Õðàíèìûå ñòðîêè íåïóñòûå è ñîñòîÿò èç ñòðî÷íûõ
ëàòèíñêèõ áóêâ.
Õåø-ôóíêöèÿ ñòðîêè äîëæíà áûòü ðåàëèçîâàíà ñ ïîìîùüþ âû÷èñëåíèÿ çíà÷åíèÿ
ìíîãî÷ëåíà ìåòîäîì Ãîðíåðà.
Íà÷àëüíûé ðàçìåð òàáëèöû äîëæåí áûòü ðàâíûì 8-ìè. Ïåðåõåøèðîâàíèå âûïîëíÿéòå
ïðè äîáàâëåíèè ýëåìåíòîâ â ñëó÷àå, êîãäà êîýôôèöèåíò çàïîëíåíèÿ òàáëèöû äîñòèãàåò
3/4.
Ñòðóêòóðà äàííûõ äîëæíà ïîääåðæèâàòü îïåðàöèè äîáàâëåíèÿ ñòðîêè â ìíîæåñòâî,
óäàëåíèÿ ñòðîêè èç ìíîæåñòâà è ïðîâåðêè ïðèíàäëåæíîñòè äàííîé ñòðîêè ìíîæåñòâó.
1_2. Äëÿ ðàçðåøåíèÿ êîëëèçèé èñïîëüçóéòå äâîéíîå õåøèðîâàíèå.*/

#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_TABLE_SIZE 8
#define REHASH_INDEX 0.75
#define HASH1 11
#define HASH2 13

using namespace std;

template<class T>
int hash1(const T& keys_array, int size) // õåø-ôóíêöèÿ 1
{
	int hash = 0;
	for (int i = keys_array.size(); i >= 0; i--) 
	{
		hash += (HASH1 * hash + keys_array[i]) % size;
	}
	return hash % size;
}

template<class T>
int hash2(const T& keys_array, int size) // õåø-ôóíêöèÿ 2
{
	int hash = 0;
	for (int i = keys_array.size(); i >= 0; i--) 
	{
		hash += (HASH2 * hash + keys_array[i]) % size;
	}
	return (hash * 2 + 1) % size;
}

template<class T>
class Hash_table_node //ýëåìåíò òàáëèöû 
{
private:
	T key; 
	bool is_deleted; 

public:
	Hash_table_node(const T& value) : key(value), is_deleted(false) {}

	const T& get_key() const 
	{
		return key;
	}

	void set_key(const T& key_value) 
	{
		key = key_value;
	}

	bool if_is_deleted() const 
	{
		return is_deleted;
	}

	void set_deleted() 
	{
		is_deleted = true;
	}

	void set_not_deleted() 
	{
		is_deleted = false;
	}
};

template<class T>
class Hash_table //õåø-òàáëèöà
{
private:
	vector<Hash_table_node<T>*> buffer; 
	int buffer_size; 
	int size; //ðåàëüíîå êîë-âî ýëåìåíòîâ â òàáëèöå
		   
	void rehash() //ïåðåõåøèðîâàíèå
	{
		int new_buffer_size = buffer_size * 2;
		vector<Hash_table_node<T>*> new_buffer(new_buffer_size, NULL);
		for (int i = 0; i < buffer_size; i++) 
		{
			if (buffer[i] != NULL && !buffer[i]->if_is_deleted()) //åñëè ýëåìåíò ñóùåñòâóåò â ñòàðîé òàáëèöå
			{
				int h1 = hash1(buffer[i]->get_key(), new_buffer_size);
				int	h2 = hash2(buffer[i]->get_key(), new_buffer_size);
				int k = 0;
				while (new_buffer[h1] != NULL && k < new_buffer_size) //èùåì ñâîáîäíóþ ÿ÷åéêó íîâîé òàáëèöû
				{
					h1 = (h1 + h2) % new_buffer_size;
					k++;
				}
				new_buffer[h1] = buffer[i];
			}
			else //åñëè ÿ÷åéêà ñòàðîé òàáëèöû ïóñòà
			{
				delete buffer[i];
			}
			buffer[i] = NULL;
		}
		buffer = new_buffer;
		buffer_size = new_buffer_size;
	}

public:
	Hash_table() : buffer(DEFAULT_TABLE_SIZE, NULL), buffer_size(DEFAULT_TABLE_SIZE), size(0) {}

	~Hash_table() 
	{
		for (int i = 0; i < buffer.size(); i++) 
		{
			delete buffer[i];
		}
	}

	bool add(const T& key_value) 
	{
		if (((double)size / (double)buffer_size) >= REHASH_INDEX) //ïðîâåðêà íà íåîáõîäèìîñòü ïåðåõåøèðîâàíèÿ
		{
			rehash();
		}
		int h1 = hash1(key_value, buffer_size);
		int	h2 = hash2(key_value, buffer_size);
		int i = 0;
		int	first_deleted_elem = -1;

		while (buffer[h1] != NULL && i < buffer_size) 
		{
			if (buffer[h1]->get_key() == key_value && !buffer[h1]->if_is_deleted()) //åñëè òàêîé êëþ÷ óæå åñòü â òàáëèöå
			{
				return false;
			}
			if (buffer[h1]->if_is_deleted() && first_deleted_elem < 0) // ñîõðàíÿåì íîìåð ïåðâîãî óäàë¸ííîãî ýëåìåíòà
			{
				first_deleted_elem = h1;
			}
			h1 = (h1 + h2) % buffer_size;
			i++;
		}

		if (first_deleted_elem < 0) //âñòàâëÿåì íîâûé êëþ÷
		{
			buffer[h1] = new Hash_table_node<T>(key_value);
		}
		else //âñòàâëÿåì íà ìåñòî ïåðâîãî óäàë¸ííîãî
		{
			buffer[first_deleted_elem]->set_key(key_value);
			buffer[first_deleted_elem]->set_not_deleted();
		}
		size++;
		return true;
	}

	bool remove(const T& key_value) 
	{
		int h1 = hash1(key_value, buffer_size);
		int	h2 = hash2(key_value, buffer_size);
		int i = 0;
		while (buffer[h1] != NULL && i < buffer_size) 
		{
			if (buffer[h1]->get_key() == key_value && !buffer[h1]->if_is_deleted()) //åñëè íàøëè ýëåìåíò
			{
				buffer[h1]->set_deleted();
				size--;
				return true;
			}
			h1 = (h1 + h2) % buffer_size;
			i++;
		}
		return false; //íå íàøëè òàêîãî ýëåìåíòà
	}

	bool has(const T& key_value) const 
	{
		int h1 = hash1(key_value, buffer_size);
		int	h2 = hash2(key_value, buffer_size);
		int i = 0;
		while (buffer[h1] != NULL && i < buffer_size) 
		{
			if (buffer[h1]->get_key() == key_value && !buffer[h1]->if_is_deleted()) //åñëè íàøëè ýëåìåíò
			{
				return true;
			}
			h1 = (h1 + h2) % buffer_size;
			i++;
		}
		return false; //íå íàøëè òàêîãî ýëåìåíòà
	}
};

int main() 
{
	Hash_table<string> hash_map;
	char command = ' ';
	string key("");
	bool status = false;

	while (cin >> command >> key)
	{
		switch (command) 
		{
		case '+':
			status = hash_map.add(key);
			break;
		case '?':
			status = hash_map.has(key);
			break;
		case '-':
			status = hash_map.remove(key);
			break;
		default:
			continue;
		}
	
		if (status)
		{
			cout << "OK" << endl;
		}
		else
		{
			cout << "FAIL" << endl;
		}
	}

	return 0;
}
