#include "PuxerCyclicList.h"

//=================================================PuxerDListItem=================================================
template <typename T>
Puxer::PuxerDListItem<T>::PuxerDListItem(T* dat) {

	this->dat = dat;
	this->next = nullptr;
	this->prev = nullptr;

}

template <typename T>
Puxer::PuxerDListItem<T>::~PuxerDListItem() {

	if (this->dat) {
		delete this->dat;
	}
}

template <typename T>
Puxer::PuxerDListItem<T>* Puxer::PuxerDListItem<T>::get_next() {
	return this->next;
}

template <typename T>
Puxer::PuxerDListItem<T>* Puxer::PuxerDListItem<T>::get_prev() {
	return this->prev;
}

//=================================================PuxerDList=================================================
template <typename T>
void Puxer::PuxerDList<T>::insert(PuxerDListItem<T>* dat) {

	if (!this->start) {

		this->start = dat;
		this->last = dat;

		return;
	}

	PuxerDListItem<T> temp = this->last;
	this->last = dat;

	dat->prev = temp;
	temp->next = dat;
}

template <typename T>
Puxer::PuxerDListItem<T>* Puxer::PuxerDList<T>::pop() {

	if (!this->last) {
		return nullptr;
	}

	if (this->start == this->last) {

		PuxerDListItem<T> temp = this->last;

		this->start = nullptr;
		this->last = nullptr;

		return temp;
	}

	PuxerDListItem<T> temp = this->last;
	this->last = temp->prev;

	return temp;
}