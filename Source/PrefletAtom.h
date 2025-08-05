// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                       _     
// |  __ \           / _| |    | |     /\   | |                     | |    
// | |__) |_ __  ___| |_| | ___| |_   /  \  | |_  ___  _ __ ___     | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | __|/ _ \| '_ ` _ \    | '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| |_| (_) | | | | | | _ | | | |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\\__|\___/|_| |_| |_|(_)|_| |_|
//                                                                         
//                                                                         
//
//   Manages atoms stored in our various preferences & settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/25/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETATOM
#define _H_PREFLETATOM

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletAtom
{
public:
	enum		ControlType {None, Button, Edit, Slider, Combo};
	enum		ValueType {Boolean, Integer, Float, String};

	// Construction/Destruction

	PrefletAtom()
	: _controlID(0), _controlType(Button), _valueType(Boolean), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(0), _slideRangeMax(0)
	{}

	// Preflets for controls

	PrefletAtom(const int idd, const ControlType ctype, const bool defaultValue, const int slideRangeMin = 0, const int slideRangeMax = 0)
	: _controlID(idd), _controlType(ctype), _valueType(Boolean), _booleanValue(false), _defaultBooleanValue(defaultValue), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(slideRangeMin), _slideRangeMax(slideRangeMax)
	{}

	PrefletAtom(const int idd, const ControlType ctype, const int defaultValue, const int slideRangeMin = 0, const int slideRangeMax = 0)
	: _controlID(idd), _controlType(ctype), _valueType(Integer), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(defaultValue), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(slideRangeMin), _slideRangeMax(slideRangeMax)
	{}

	PrefletAtom(const int idd, const ControlType ctype, const float defaultValue, const int slideRangeMin = 0, const int slideRangeMax = 0)
	: _controlID(idd), _controlType(ctype), _valueType(Float), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(defaultValue), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(slideRangeMin), _slideRangeMax(slideRangeMax)
	{}

	PrefletAtom(const int idd, const ControlType ctype, const wstring & defaultValue, const int slideRangeMin = 0, const int slideRangeMax = 0)
	: _controlID(idd), _controlType(ctype), _valueType(String), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(defaultValue), _control(0),
	_slideRangeMin(slideRangeMin), _slideRangeMax(slideRangeMax)
	{}

	// Generic preflets (not tied to a control)

	PrefletAtom(const bool defaultValue)
	: _controlID(0), _controlType(None), _valueType(Boolean), _booleanValue(false), _defaultBooleanValue(defaultValue), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(0), _slideRangeMax(0)
	{}

	PrefletAtom(const int defaultValue)
	: _controlID(0), _controlType(None), _valueType(Integer), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(defaultValue), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(0), _slideRangeMax(0)
	{}

	PrefletAtom(const float defaultValue)
	: _controlID(0), _controlType(None), _valueType(Float), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(defaultValue), _stringValue(_T("")), _defaultStringValue(_T("")), _control(0),
	_slideRangeMin(0), _slideRangeMax(0)
	{}

	PrefletAtom(const wstring & defaultValue)
	: _controlID(0), _controlType(None), _valueType(String), _booleanValue(false), _defaultBooleanValue(false), _integerValue(0),
	_defaultIntegerValue(0), _floatValue(0), _defaultFloatValue(0), _stringValue(_T("")), _defaultStringValue(defaultValue), _control(0),
	_slideRangeMin(0), _slideRangeMax(0)
	{}

	PrefletAtom(const PrefletAtom & rhs) {operator=(rhs);}

virtual	~PrefletAtom() {}

	// Operators

inline		PrefletAtom &	operator = (const PrefletAtom & rhs)
				{
					if (this != &rhs)
					{
						_controlID = rhs._controlID;
						_controlType = rhs._controlType;
						_valueType = rhs._valueType;
						_booleanValue = rhs._booleanValue;
						_defaultBooleanValue = rhs._defaultBooleanValue;
						_integerValue = rhs._integerValue;
						_defaultIntegerValue = rhs._defaultIntegerValue;
						_stringValue = rhs._stringValue;
						_defaultStringValue = rhs._defaultStringValue;
						_floatValue = rhs._floatValue;
						_defaultFloatValue = rhs._defaultFloatValue;
						_control = const_cast<CWnd *>(rhs._control);
						_slideRangeMin = rhs._slideRangeMin;
						_slideRangeMax = rhs._slideRangeMax;
					}
					return *this;
				}

	// Implementation

virtual		void		createControl();
virtual		void		destroyControl();
virtual		void		readFromRegistry(const wstring & name, const wstring & subkey);
virtual		void		writeToRegistry(const wstring & name, const wstring & subkey) const;
virtual		void		readFromControl(const wstring & name);
virtual		void		writeToControl(const wstring & name);

	// Accessors

inline		int &		controlID()			{return _controlID;}
inline	const	int		controlID() const		{return _controlID;}
inline		ControlType &	controlType()			{return _controlType;}
inline	const	ControlType	controlType() const		{return _controlType;}
inline		ValueType &	valueType()			{return _valueType;}
inline	const	ValueType	valueType() const		{return _valueType;}
inline		bool &		booleanValue()			{ASSERT(valueType() == Boolean); return _booleanValue;}
inline	const	bool		booleanValue() const		{ASSERT(valueType() == Boolean); return _booleanValue;}
inline		bool &		defaultBooleanValue()		{return _defaultBooleanValue;}
inline	const	bool		defaultBooleanValue() const	{return _defaultBooleanValue;}
inline		int &		integerValue()			{ASSERT(valueType() == Integer); return _integerValue;}
inline	const	int		integerValue() const		{ASSERT(valueType() == Integer); return _integerValue;}
inline		int &		defaultIntegerValue()		{return _defaultIntegerValue;}
inline	const	int		defaultIntegerValue() const	{return _defaultIntegerValue;}
inline		float &		floatValue()			{ASSERT(valueType() == Float); return _floatValue;}
inline	const	float		floatValue() const		{ASSERT(valueType() == Float); return _floatValue;}
inline		float &		defaultFloatValue()		{return _defaultFloatValue;}
inline	const	float		defaultFloatValue() const	{return _defaultFloatValue;}
inline		wstring &	stringValue()			{ASSERT(valueType() == String); return _stringValue;}
inline	const	wstring &	stringValue() const		{ASSERT(valueType() == String); return _stringValue;}
inline		wstring &	defaultStringValue()		{return _defaultStringValue;}
inline	const	wstring &	defaultStringValue() const	{return _defaultStringValue;}
inline		CWnd *&		control()			{return _control;}
inline	const	CWnd *		control() const			{return _control;}
inline		int &		slideRangeMin()			{return _slideRangeMin;}
inline	const	int		slideRangeMin() const		{return _slideRangeMin;}
inline		int &		slideRangeMax()			{return _slideRangeMax;}
inline	const	int		slideRangeMax() const		{return _slideRangeMax;}

private:
	// Data members

		int		_controlID;
		ControlType	_controlType;
		ValueType	_valueType;
		bool		_booleanValue;
		bool		_defaultBooleanValue;
		int		_integerValue;
		int		_defaultIntegerValue;
		wstring		_stringValue;
		wstring		_defaultStringValue;
		float		_floatValue;
		float		_defaultFloatValue;
		CWnd *		_control;
		int		_slideRangeMin;
		int		_slideRangeMax;
};

#endif // _H_PREFLETATOM
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAtom.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
