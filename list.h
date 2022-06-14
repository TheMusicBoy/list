#ifndef _TMB_LIST_
#define _TMB_LIST_

#include <xmemory>

namespace tmb {

	template<typename _Ty>
	struct _Node {
		_Ty _val;

		_Node* next = NULL;
		_Node* prev = NULL;
	};

	template<typename _Ty, typename _Alloc = std::allocator<_Node<_Ty>>>
	class list {
	private:
		
        using Node = _Node<_Ty>;
		using Nodeptr = Node*;

		Nodeptr _begin, _end;
		size_t _capacity;

		_Alloc alloc;

	public:

        class iterator {
        private:

            using difference_type = int;

            Nodeptr _Ptr;

        public:

            iterator() {}

            iterator(Nodeptr ptr) {
                _Ptr = ptr;
            }

            iterator(const iterator& op) {
                _Ptr = op._Ptr;
            }

            iterator& operator++(int) {
                _Ptr = _Ptr->next;

                return *this;
            }

            iterator operator++() {
                iterator buff = *this;
                _Ptr = _Ptr->next;

                return buff;
            }

            iterator& operator--(int) {
                _Ptr = _Ptr->prev;

                return *this;
            }

            iterator operator--() {
                iterator buff = *this;
                _Ptr = _Ptr->prev;

                return buff;
            }

            _Ty* operator->() {
                return &(**this);
            }

            _Ty& operator*() {
                return _Ptr->_val;
            }

            bool operator==(iterator op) {
                return _Ptr == op._Ptr;
            }

            bool operator!=(iterator op) {
                return _Ptr != op._Ptr;
            }
        };

		list() {
            _begin = NULL;
            _end = NULL;
			_capacity = 0;
		}

		list(size_t new_size) : list() {
			_capacity = new_size;

            if (_capacity == 0) return;

            _begin = alloc.allocate(1);

            _end = _begin;

			for (size_t iter = 1; iter < _capacity; iter++) {
				_end->next = alloc.allocate(1);
                *_end->next = Node();
				_end->next->prev = _end;
				_end = _end->next;
			}
		}

        list(size_t new_size, _Ty ex) : list(new_size) {
            for (auto& el : *this)
                el = ex;
        }

        ~list() {
            clear();
        }

        _Ty pop_back() {
            _Ty res = _end->_val;

            _end = _end->prev;
            alloc.deallocate(_end->next, 1);
            _end->next = NULL;

            _capacity--;

            return res;
        }

        _Ty pop_front() {
            _Ty res = _begin->_val;

            _begin = _begin->next;
            alloc.deallocate(_begin->prev, 1);
            _begin->prev = NULL;

            _capacity--;

            return res;
        }

        void push_back() {
            _end->next = alloc.allocate();
            *_end->next = Node();
            _end->next->prev = _end;
            _end = _end->next;

            _capacity++;
        }

        void push_back(_Ty ex) {
            push_back();
            _end->_val = ex;
        }

        void push_front() {
            _begin->prev = alloc.allocate();
            *_begin->prev = Node();
            _begin->prev->next = _begin;
            _begin = _begin->prev;

            _capacity++;
        }

        void push_front(_Ty ex) {
            push_front();
            _begin->val = ex;
        }

        void clear() {
            Nodeptr pos = _begin;
            while (pos != NULL) {
                Nodeptr next = pos->next;
                alloc.deallocate(pos, 1);
                pos = next;
            }
            _capacity = 0;
        }

        iterator begin() {
            return iterator(_begin);
        }

        iterator end() {
            return iterator(NULL);
        }
	};
}

#endif