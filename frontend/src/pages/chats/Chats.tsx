import {  Gradient } from "@/components/UI"
import styles from "./Chats.module.css"
import { IChat,  MessageType } from "@/types"
import NotAuthPage from "@/pages/notAuth/NotAuth"

import { useFetching } from "@/hooks/useFetch"
import { useUserStore } from "@/state/user"
// import { useUserStore } from "@/state/user"

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

const messages: MessageType[] = [
    {
        text: "Hi",
        author: "Ivan"
    },
    {
        text: "How are u?",
        author: "Ivan"
    },
    {
        text: "Hello!",
        author: "me"
    },
    {
        text: "Ok",
        author: "Ivan"
    }
]

const ChatsPage = (): JSX.Element => {
    const isAuth = useUserStore(state => state.auth)
    const [friends, setFriends] = useState<IChat[] | null>(null)

    if (!isAuth) {
        return <NotAuthPage />
    }

    const [fetchUsers, _, error] = useFetching(async () => {
        const data = await ChatService.getAll()
        setFriends(data)
    })

    useEffect(() => {
        fetchUsers();
    }, []);

    if (error) {
        return <div>{error}</div>
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
                />
                <Chat className={styles.messages} data={messages} />
                <Settings className={styles.settings} />
                <MessagePrompt className={styles.prompt} />
            </div>
        </div>
    )
}

export default ChatsPage
