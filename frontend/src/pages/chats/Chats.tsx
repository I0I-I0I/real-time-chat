import { Gradient } from "@/components/UI"
import styles from "./Chats.module.css"
import { IChat } from "@/types"
import NotAuthPage from "@/pages/notAuth/NotAuth"

import { useFetching } from "@/hooks/useFetch"
import { useUserStore } from "@/state/user"
import { useChatStore } from "@/state/chat"

import {
    ChatInfo,
    AddChat,
    ChatsList,
    Settings,
    MessagePrompt,
    Chat,
    createNewChatData
} from "@/components"
import { useEffect } from "react"
import ChatService from "@/api/ChatService"
import MessageService from "@/api/MessageService"
import { useChatsListStore } from "@/state/all_chats"

const ChatsPage = (): JSX.Element => {
    const setAuth = useUserStore(state => state.setAuth)
    const isAuth = useUserStore(state => state.auth)
    const chats = useChatsListStore(state => state.data)
    const setChats = useChatsListStore(state => state.setChatsList)
    const currentChat = useChatStore(state => state.data)
    const setCurrentChat = useChatStore(state => state.setCurrentChat)
    const setMessages = useChatStore(state => state.setMessages)
    const currentUserId = useUserStore(state => state.data?.id)

    const [fetchUsers,, fetchUsersError] = useFetching(async () => {
        if (!currentUserId) return
        const data = await ChatService.getAll(currentUserId)
        if (data === null) {
            setChats([])
            return
        }
        setChats(data)
    })

    const [fetchMessages,, fetchMessagesError] = useFetching(async () => {
        if (currentChat === null) return;
        let data = await MessageService.getAll(currentChat.id)
        if (data == null) data = []
        setMessages(data)
    })

    const createNewChat = async (user: createNewChatData) => {
        if (!currentUserId) {
            console.error("currentUserId is null")
            return
        }
        const newChat = await ChatService.createOne(
            { name: user.name, lastMessage: "0" },
            currentUserId
        )
        if (!newChat) return

        const status = await ChatService.addUserToChat(newChat.id, user.id)
        if (status !== 200) {
            ChatService.removeOne(newChat.id)
            console.error("Can't add user to chat")
            return
        }

        const newChats = await ChatService.getAll(currentUserId)
        if (!newChats) return
        setChats(newChats)
    }

    const removeChat = async (chatId: number) => {
        if (!chatId) return
        const status = await ChatService.removeOne(chatId)
        if (status !== 200) return
        if (!chats) return
        const newChats = chats.filter(chat => chat.id !== chatId)
        newChats === null ? setChats([]) : setChats(newChats)
        setMessages([])
    }

    const onClickChatsListItem = (chat: IChat) => {
        setCurrentChat(chat)
    }

    const disableAuth = () => {
        setAuth(false)
    }

    useEffect(() => {
        fetchUsers();
    }, []);

    useEffect(() => {
        fetchMessages()
    }, [currentChat]);

    if (!isAuth) {
        return <NotAuthPage />
    }

    if (fetchUsersError) {
        return <div>{fetchUsersError}</div>
    }

    if (fetchMessagesError) {
        return <div>{fetchMessagesError}</div>
    }

    return (
        <div className={styles.wrapper}>
            <Gradient />
            <div className={styles.container}>
                <AddChat className={styles.add_friends} createNewChat={createNewChat} />
                <ChatInfo className={styles.info} />
                <ChatsList
                    data={chats}
                    className={styles.list}
                    onClick={onClickChatsListItem}
                    removeChat={removeChat}
                />
                <Chat className={styles.messages} />
                <Settings className={styles.settings} onClickLogout={disableAuth} />
                <MessagePrompt className={styles.prompt} />
            </div>
        </div>
    )
}

export default ChatsPage
