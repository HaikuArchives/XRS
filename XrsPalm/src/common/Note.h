#ifndef _NOTE_H_
#define _NOTE_H_

class Note
{
	public:
	
			Note();
		bool 	getValue();
		void	setValue(bool);
		void	Swap();
		float	getVolume();
		void	setVolume(float);
		void	setOct(int);
		int	getOct();
		long	getNote(){return note;};
		void	setNote(long v){note=v;};
	private:

		bool	value;
		float   volume;
		int	oct;  // maschera di bit che decide : ottava,slide,accento
		long	note;
};

#endif