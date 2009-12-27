#ifndef NULLITY_CHANNEL_H
#define NULLITY_CHANNEL_H

namespace nullity {
	class ICommInterface;
	class IChannel;
	class IMessage;

	/// Manager for communications and channels for use by objects.
	class ICommInterface {
	public:

	};

	/// Channel used for communication between objects, realities and times.
	class IChannel  {
	public:


	};

	/// A message that can be sent on a channel.
	class IMessage {
	public:

		/// Deletes the message and all resources used by it.
		virtual void		Destroy() = 0;
	};

}

#endif