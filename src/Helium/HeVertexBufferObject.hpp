#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	template <class T>
	class HeVertexBufferObject
	{
	public:
		enum BufferType { VERTEX_BUFFER, INDEX_BUFFER, COLOR_BUFFER, UV_BUFFER };

	public:
		HeVertexBufferObject(BufferType bufferType)
			: bufferType(bufferType)
		{
		}

		~HeVertexBufferObject()
		{
		}

		inline unsigned int ID() { return id; }

		void Initialize(unsigned int attributeIndex)
		{
			this->attributeIndex = attributeIndex;

			glGenBuffers(1, &id);

			CheckGLError();
		}

		void Terminate()
		{
			glDeleteBuffers(1, &id);

			CheckGLError();
		}

		void Bind()
		{
			if (bufferType == INDEX_BUFFER)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, id);
			}

			if (dirty)
			{
				Upload();

				dirty = false;
			}
			CheckGLError();
		}

		void Unbind()
		{
			if (bufferType == INDEX_BUFFER)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			CheckGLError();
		}

		inline size_t Size() { return elements.size(); }

		void AddElement(const T& element)
		{
			elements.push_back(element);

			dirty = true;
		}

		const T& GetElement(int index)
		{
			return elements[index];
		}

		bool SetElement(int index, const T& element)
		{
			if (index >= elements.size())
				return false;

			elements[index] = element;

			dirty = true;

			return true;
		}

		void Upload()
		{
			if (elements.size() == 0)
				return;

			if (bufferType == VERTEX_BUFFER)
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(T) * elements.size(), &elements[0], GL_STATIC_DRAW);
				CheckGLError();

				glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				CheckGLError(); 
				
				glEnableVertexAttribArray(attributeIndex);
				CheckGLError();
			}
			else if (bufferType == INDEX_BUFFER)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * elements.size(), &elements[0], GL_STATIC_DRAW);

				CheckGLError();
			}
			if (bufferType == COLOR_BUFFER)
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(T) * elements.size(), &elements[0], GL_STATIC_DRAW);
				CheckGLError();

				glVertexAttribPointer(attributeIndex, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
				CheckGLError(); 
				
				glEnableVertexAttribArray(attributeIndex);
				CheckGLError();
			}
			else if (bufferType == UV_BUFFER)
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(T) * elements.size(), &elements[0], GL_STATIC_DRAW);
				CheckGLError();

				glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
				CheckGLError();

				glEnableVertexAttribArray(attributeIndex);
				CheckGLError();
			}
		}

	private:
		bool dirty = true;

		BufferType bufferType;
		unsigned int id;
		unsigned int attributeIndex;
		vector<T> elements;
	};

}