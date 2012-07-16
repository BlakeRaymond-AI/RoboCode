#ifndef OBSERVER_HEADER_GUARD
#define OBSERVER_HEADER_GUARD

class Observer
{
	public:
	void addSignal(Signal* signal)
	{
		if(signal == NULL)
			return;
		if(signalList.contains(signal))
			return;
			
		signalList.addElement(signal);
	}
	
	void removeSignal(Signal* signal)
	{
		if(signal == NULL)
			return;
			
		int index = signalList.indexOf(signal);
		if(index != -1)
		{
			signalList.removeElementAt(index);
		}
	}
	
	void update()
	{
		for(int i=0; i<signalList.size(); ++i)
		{
			signalList.get(i)->read();
		}
	}
	
	private:
	Vector<Signal*> signalList;
};

Observer OBSERVER;

#endif