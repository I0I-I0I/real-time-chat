import ChatService from "@/api/ChatService"
import MessageService from "@/api/MessageService"
import {
    AddChat,
    Chat,
    ChatInfo,
    ChatsList,
    createNewChatData,
    MessagePrompt,
    Settings
} from "@/components"
import { Gradient } from "@/components/UI"
import { useFetching } from "@/hooks/useFetch"
import NotAuthPage from "@/pages/notAuth/NotAuth"
import { useChatsListStore } from "@/state/all_chats"
import { useChatStore } from "@/state/chat"
import { useUserStore } from "@/state/user"
import { IChat } from "@/types"
import { useEffect, useRef } from "react"
import styles from "./ChatsPage.module.css"
import AuthService from "@/api/AuthService"

const ChatsPage = (): JSX.Element => {
    const setAuth = useUserStore(state => state.setAuth)
    const isAuth = useUserStore(state => state.auth)
    const chats = useChatsListStore(state => state.data)
    const setChats = useChatsListStore(state => state.setChatsList)
    const currentChat = useChatStore(state => state.data)
    const setCurrentChat = useChatStore(state => state.setCurrentChat)
    const setMessages = useChatStore(state => state.setMessages)
    const currentUser = useUserStore(state => state.data)
    const chatInfoRef = useRef(null)
    const addChatRef = useRef(null)

    useEffect(() => {
        const interval = setInterval(async () => {
            try {
                if (!currentUser?.id) return;
                const chats = await ChatService.getAll(currentUser?.id);
                if (chats) {
                    setChats(chats);
                }
                if (!currentChat) return;
                const currentChatMessages = await MessageService.getAll(currentChat.id);
                if (currentChatMessages) {
                    setMessages(currentChatMessages);
                }
            } catch (error) {
                console.error("Error fetching data:", error);
            }
        }, 3000);

        return () => {
            clearInterval(interval);
        };
    }, [currentUser?.id, currentChat]);

    const [fetchChats,, fetchChatsError] = useFetching(async () => {
        if (!currentUser?.id) return
        const data = await ChatService.getAll(currentUser?.id)
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

    useEffect(() => {
        fetchChats()
    }, [currentUser?.id]);

    useEffect(() => {
        fetchMessages()
    }, [currentChat?.id]);

    useEffect(() => {
        // @ts-ignore
        addChatRef.current?.scrollIntoView({ behavior: "smooth" })
    }, [])


    const createNewChat = async (user: createNewChatData) => {
        if (!currentUser?.id) {
            console.error("currentUser?.id is null")
            return
        }
        const newChat = await ChatService.createOne(
            { name: user.name, name2: currentUser.login, lastMessage: "0" },
            currentUser?.id
        )
        if (!newChat) return

        const status = await ChatService.addUserToChat(newChat.id, user.id)
        if (status !== 200) {
            ChatService.removeOne(newChat.id)
            console.error("Can't add user to chat")
            return
        }

        const newChats = await ChatService.getAll(currentUser?.id)
        if (!newChats) return
        setChats(newChats)
    }

    const onClickCloseMobile = () => {
        // @ts-ignore
        addChatRef.current?.scrollIntoView({ behavior: "smooth" })
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
        // @ts-ignore
        chatInfoRef.current?.scrollIntoView({ behavior: "smooth" })
    }

    const onLogout = () => {
        const hash = localStorage.getItem("token")
        if (hash) {
            if (!currentUser?.login) return
            AuthService.logout(currentUser.login, hash)
            localStorage.removeItem("token")
        }
        setAuth(false)
    }

    if (!isAuth) {
        return <NotAuthPage />
    }

    if (fetchChatsError) {
        return <div>{fetchChatsError}</div>
    }

    if (fetchMessagesError) {
        return <div>{fetchMessagesError}</div>
    }

    return (
        <div className={styles.wrapper}>
            <Gradient />
            <div className={styles.container}>
                <AddChat className={styles.add_friends} createNewChat={createNewChat} ref={addChatRef} />
                <ChatInfo className={styles.info} ref={chatInfoRef} onClickCloseMobile={onClickCloseMobile} />
                <ChatsList
                    data={chats}
                    className={styles.list}
                    onClick={onClickChatsListItem}
                    removeChat={removeChat}
                />
                <Chat className={styles.chat} />
                <Settings className={styles.settings} onClickLogout={onLogout} />
                <MessagePrompt className={styles.prompt} />
            </div>
        </div>
    )
}

export default ChatsPage
