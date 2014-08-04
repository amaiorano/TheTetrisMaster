#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#ifndef NULL
#define NULL 0
#endif

// This template class can be used to easily create a singleton class.
// To use it, simply derive your class from "Singleton<YOUR_CLASS_NAME>"
// and place your class's constructor in protected (not private).
//
// To get the single instance of your class, call the static Instance()
// through your class, ex: "YOUR_CLASS_NAME::Instance()"

template <class T>
class Singleton
{
    public:
		// Returns true if the singleton instance has been created, false otherwise
		inline static bool IsInstantiated();

		// Retrieves the single instance (creates it if it doesn't exist)
        inline static T& Instance();

    protected:		
        Singleton();
		~Singleton(); // Protected so instance cannot be deleted from outside

	private:
		// Declare an empty child class of T to avoid the need for friendship.
		// Type T only needs to hide its constructor in the protected section.
		struct SingletonChild : public T
		{
		};

		// Declare a destroyer for the singleton instance
		template <class T>
		class InstanceDestroyer
		{
			public:
				InstanceDestroyer() : m_pInstance(NULL) {}
				~InstanceDestroyer() { delete m_pInstance; }
				void SetInstance(T* pInstance) { m_pInstance = pInstance; }

			private:
				T* m_pInstance;
		};

    private:
		static SingletonChild* m_pInstance;

		// This destroyer instance will be destructed on program exit, deleting 
		// the singleton instance with it (if any)
		static InstanceDestroyer<T> m_destroyer;
};


// C++ Standard: must use keyword 'typename' to specify a type within a template

template <class T>
typename Singleton<T>::SingletonChild* Singleton<T>::m_pInstance = NULL;

template <class T>
typename Singleton<T>::InstanceDestroyer<T> Singleton<T>::m_destroyer;

template <class T>
Singleton<T>::Singleton()
{
}

template <class T>
Singleton<T>::~Singleton()
{
}

template <class T>
inline bool Singleton<T>::IsInstantiated()
{
	return (m_pInstance != NULL);
}

template <class T>
inline T& Singleton<T>::Instance()
{
    if (!m_pInstance)
	{
		m_pInstance = new SingletonChild;
		m_destroyer.SetInstance( m_pInstance );
	}

	return static_cast<T&>(*m_pInstance); // Perfectly safe cast
}

#endif // _SINGLETON_H_
