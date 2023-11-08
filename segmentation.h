#pragma once
#include "opcvwrapper.h"

class NoduleRec final
{
public:
	explicit NoduleRec(const std::string& path)
	{
		loadImage(path, original);
	};

	explicit NoduleRec(const NoduleRec& n)
	{
		if (&n != this)
		{
			this->original = n.original;
			this->final = n.final;
			this->edges = n.edges;
			this->contours = n.contours;
			this->hierarchy = n.hierarchy;
		}

	}
	
	NoduleRec& operator=(const NoduleRec& n)
	{
		if (&n != this)
		{
			this->original = n.original;
			this->final = n.final;
			this->edges = n.edges;
			this->contours = n.contours;
			this->hierarchy = n.hierarchy;
		}
		return *this;
	}

	virtual ~NoduleRec()
	{
		resetInternal();
	};
	
	const Mat& getOriginalImg() const { return original; };
	const Mat& getFinalImg() const { return final; };
	const Mat& getEdgesImg() const { return edges; };
	const bool ErrorInOriginalLoading() const { return original.empty(); };
	RoiAretype getRoi() const { return contours; };
	std::vector<Vec4i> gethierarchy() const { return hierarchy; };
	bool findContornos(int threshold);
	void HighlightRoi();

	void loadNewImage(const std::string& path)
	{	
		Mat tmp;
		loadImage(path, tmp);
		resetInternal();
		tmp.copyTo(original);
		tmp.release();
	}

	void resetInternal()
	{
		contours.clear();
		hierarchy.clear();
		original.release();
		final.release();
		edges.release();
	}
		
private:
	Mat original;
	Mat final;
	Mat edges;
	RoiAretype contours;
	std::vector<Vec4i> hierarchy;
};

class EyesDetector
{
public:
	explicit EyesDetector(NoduleRec& n): n(n){ };
	virtual ~EyesDetector() {};
	Mat findEyes(int tol, int option);
	std::vector<Rect> removeNonEyes(std::vector<Rect>& eyes,int tol);

private:

	inline int calculateRadius(const Rect& r) const
	{
		int sum = r.width + r.height;
		return cvRound( sum*0.25);
	}

	inline int getYCenter(const Rect& r) const
	{
		return r.y + r.height / 2;
	}

	inline int getXCenter(const Rect& r) const
	{
		return r.x + r.width / 2;
	}

	NoduleRec& n;
};

