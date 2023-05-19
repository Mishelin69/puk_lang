#pragma once

namespace Puxer {

	template<typename T>
	class PuxerDListItem {

	public:

		PuxerDListItem<T>* prev;
		PuxerDListItem<T>* next;
		T* dat;

	public:

		PuxerDListItem(T* dat);
		PuxerDListItem(PuxerDListItem<T>& dat) = default;
		~PuxerDListItem();

		PuxerDListItem<T>* get_prev();
		PuxerDListItem<T>* get_next();

	};

	template<typename T>
	class PuxerDList {

	private:

		PuxerDListItem<T>* start;
		PuxerDListItem<T>* last;

	public:

		PuxerDList() = default;
		PuxerDList(PuxerDList<T>& dat) = delete;
		~PuxerDList() = default;

		void insert(PuxerDListItem<T>* dat);

		PuxerDListItem<T>* pop();

	};

}
