#pragma once
#include <string>
namespace CodeTest {
	struct Record {
		/*������, 0
		��Լ����, 
		����������, 
		��Լ�ڽ������Ĵ���, 
		���¼�, 
		�ϴν����, 5
		������, 
		��ֲ���, 
		����, 
		��߼�, 
		��ͼ�, 10
		����, 
		�ɽ����, 
		�ֲ���, 
		������, 
		���ν����, 15
		��ͣ���, 
		��ͣ���, 
		����ʵ��, 
		����ʵ��, 
		����޸�ʱ��, 20
		����޸ĺ���, 
		�����һ, 
		������һ, 
		������һ, 
		������һ, 25
		����۶�, 
		��������, 
		�����۶�, 
		��������, 
		�������, 30
		��������, 
		��������, 
		��������, 
		�������, 
		��������, 35
		��������, 
		��������, 
		�������, 
		��������, 
		��������, 40
		��������, 
		���վ���, 
		ҵ������
		*/
		//std::string tradingDay;
		//std::string contractNumber;
		//std::string exchange;
		//std::string contractNumberInExchange;
		//double latestPrice;


	};

	enum RecordFields
	{
		LASTMODIFYTIME = 20,
		LASTMODIFYMILS = 21,
		BIDPRICE1 = 22,
		BIDVOL1 = 23,
		ASKPRICE1 = 24,
		ASKVOL1 = 25
	};
}