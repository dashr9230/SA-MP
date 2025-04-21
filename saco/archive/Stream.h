
#pragma once

// This file defines a basic interface for data streams
// and provides implementations for a filebased stream
// as well as as a memory based stream.

#include <stdio.h>
#include <stdlib.h>

class CAbstractStream
{
public:
	enum eSeek
	{
		SeekStart,
		SeekCurrent,
		SeekEnd,
	};

	CAbstractStream() {}

	virtual ~CAbstractStream() {}

	virtual unsigned int Read(void *pbBuffer, int nNumberOfBytes) = 0;

	virtual unsigned int Read(void *pbBuffer, int nElementSize, int nNumberOfElements)
	{
		return Read(pbBuffer, nNumberOfElements * nElementSize);
	}

	virtual unsigned int Write(const void *pbBuffer, int nNumberOfBytes) = 0;

	virtual unsigned int Write(const void *pbBuffer, int nElementSize, int nNumberOfElements)
	{
		return Write(pbBuffer, nNumberOfElements * nElementSize);
	}

	virtual void Seek(int nOffset, eSeek seekMode) = 0;

	virtual void Seek(int nOffset)
	{
		Seek(nOffset, SeekStart);
	}

	virtual unsigned int Tell() = 0;
};

class CFileStream :
	public CAbstractStream
{
protected:
	FILE *m_fiFile;

public:
	enum eMode
	{
		ModeRead,
		ModeReadWrite,
		ModeAppend,
		ModeWrite,
	};

	enum eType
	{
		TypeText,
		TypeBinary,
	};

	CFileStream()
	{
		m_fiFile = NULL;
	}

	CFileStream(const char *szFilename, eType type, eMode mode)
	{
		m_fiFile = NULL;
		Open(szFilename, type, mode);
	}

	virtual ~CFileStream()
	{
		if (m_fiFile)
			Close();
	}

	virtual bool Open(const char *szFilename, eType type, eMode mode)
	{
		if (m_fiFile)
			Close();

		char szMode[4];
		int nPos = 0;
		szMode[0] = szMode[1] = szMode[2] = szMode[3] = 0;

		switch(mode)
		{
			case ModeRead:
				szMode[nPos++] = 'r'; break;
			case ModeWrite:
				szMode[nPos++] = 'w'; break;
			case ModeReadWrite:
				szMode[nPos++] = 'r'; szMode[nPos++] = '+'; break;
			case ModeAppend:
				szMode[nPos++] = 'a'; break;
		}

		switch(type)
		{
			case TypeText:
				szMode[nPos++] = 't'; break;
			case TypeBinary:
				szMode[nPos++] = 'b'; break;
		}

		m_fiFile = fopen(szFilename, szMode);
		if (!m_fiFile)
			return false;

		return true;
	}

	virtual bool IsOpen()
	{
		return (m_fiFile != NULL);
	}

	virtual void Close()
	{
		if (m_fiFile)
		{
			fclose(m_fiFile);
			m_fiFile = NULL;
		}
	}

	virtual unsigned int Read(void *pbBuffer, int nNumberOfBytes)
	{
		return (unsigned int)fread(pbBuffer, 1, nNumberOfBytes, m_fiFile);
	}

	virtual unsigned int Read(void *pbBuffer, int nElementSize, int nNumberOfElements)
	{
		return (unsigned int)fread(pbBuffer, nElementSize, nNumberOfElements, m_fiFile);
	}

	virtual unsigned int Write(const void *pbBuffer, int nNumberOfBytes)
	{
		return (unsigned int)fwrite(pbBuffer, 1, nNumberOfBytes, m_fiFile);
	}

	virtual unsigned int Write(const void *pbBuffer, int nElementSize, int nNumberOfElements)
	{
		return (unsigned int)fwrite(pbBuffer, nElementSize, nNumberOfElements, m_fiFile);
	}

	virtual void Seek(int nOffset, eSeek seekMode)
	{
		int mode = 0;
		switch (seekMode)
		{
			case SeekCurrent:
				mode = SEEK_CUR; break;
			case SeekStart:
				mode = SEEK_SET; break;
			case SeekEnd:
				mode = SEEK_END; break;
		}

		fseek(m_fiFile, nOffset, seekMode);
	}

	virtual unsigned int Tell()
	{
		return (unsigned int)ftell(m_fiFile);
	}

};

class CMemoryStream :
	public CAbstractStream
{
protected:
	unsigned char *m_pbBuffer;
	unsigned int m_nOffset;
	unsigned int m_nLength;
	bool m_bCopy;

public:
	CMemoryStream(unsigned char *pbBuffer, unsigned int nLength, bool bCopy = true)
	{
		m_bCopy = bCopy;
		m_nOffset = 0;
		m_nLength = nLength;

		if (bCopy)
		{
			m_pbBuffer = (unsigned char *)malloc(nLength);
			memcpy(m_pbBuffer, pbBuffer, nLength);
		}
		else
		{
			m_pbBuffer = pbBuffer;
		}
	}

	virtual ~CMemoryStream()
	{
		if (m_bCopy && m_pbBuffer)
		{
			free(m_pbBuffer);
		}
	}

	virtual unsigned int Read(void *pbBuffer, int nNumberOfBytes)
	{
		if (m_nOffset + nNumberOfBytes > m_nLength)
			nNumberOfBytes = m_nLength - m_nOffset;

		memcpy(pbBuffer, m_pbBuffer + m_nOffset, nNumberOfBytes);
		m_nOffset += nNumberOfBytes;

		return nNumberOfBytes;
	}

	virtual unsigned int Write(const void *pbBuffer, int nNumberOfBytes)
	{
		if (m_nOffset + nNumberOfBytes > m_nLength)
			nNumberOfBytes = m_nLength - m_nOffset;

		memcpy(m_pbBuffer + m_nOffset, pbBuffer, nNumberOfBytes);
		m_nOffset += nNumberOfBytes;

		return nNumberOfBytes;
	}

	virtual void Seek(int nOffset, eSeek seekMode)
	{
		switch(seekMode)
		{
			case SeekStart:
				m_nOffset = nOffset; break;
			case SeekCurrent:
				m_nOffset += nOffset; break;
			case SeekEnd:
				m_nOffset = m_nLength + nOffset; break;
		}

		if (m_nOffset > m_nLength)
			m_nOffset = m_nLength;
	}

	virtual unsigned int Tell()
	{
		return m_nOffset;
	}
};
