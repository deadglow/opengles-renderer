#pragma once

class reIListener
{
public:
	virtual void OnRegister() { }
	virtual void OnDeregister() { }

	// called the first time it's updated
	virtual void OnBegin() { }

	// called every game tick
	virtual void OnUpdate() { }
	// called after every update has been called
	virtual void OnPostUpdate() { }

	// called every frame
	virtual void OnPreRender() { }
	virtual void OnPostRender() { }

	bool m_begun;
};