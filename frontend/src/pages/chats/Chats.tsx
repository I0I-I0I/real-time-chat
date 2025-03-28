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
    Chat
} from "@/components"
import { useEffect, useState } from "react"
import ChatService from "@/api/ChatService"
import MessageService from "@/api/MessageService"

const ChatsPage = (): JSX.Element => {
    const isAuth = useUserStore(state => state.auth)
    const [friends, setFriends] = useState<IChat[] | null>(null)
    const currentChat = useChatStore(state => state.data)
    const setCurrentChat = useChatStore(state => state.setCurrentChat)
    const setMessages = useChatStore(state => state.setMessages)
    const currentUserId = useUserStore(state => state.data?.id)

    if (!isAuth) {
        return <NotAuthPage />
    }

    const [fetchUsers,, fetchUsersError] = useFetching(async () => {
        const data = await ChatService.getAll()
        setFriends(data)
    })

    const [fetchMessages,, fetchMessagesError] = useFetching(async () => {
        if (currentChat === null) return;
        let data = await MessageService.getAll(currentChat.id)
        if (data == null) data = []
        setMessages(data)
    })

    const createNewChat = (name: string) => {
        const data = {
            name: name,
            lastMessage: "0"
        }
        if (!currentUserId) {
            console.log("currentUserId is null")
            return
        }
        ChatService.createOne(data, currentUserId)
    }

    const onClickChatsListItem = (chat: IChat) => {
        setCurrentChat(chat)
    }

    useEffect(() => {
        fetchUsers();
    }, []);

    useEffect(() => {
        fetchMessages()
    }, [currentChat]);

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
                <AddChat className={styles.add_friends} />
                <ChatInfo className={styles.info} />
                <ChatsList
                    data={friends}
                    className={styles.list}
                    onClick={onClickChatsListItem}
                    createNewChat={createNewChat}
                />
                <Chat className={styles.messages} />
                <Settings className={styles.settings} />
                <MessagePrompt className={styles.prompt} />
            </div>
        </div>
    )
}

export default ChatsPage
