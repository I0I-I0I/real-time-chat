import { Gradient } from "@/components/UI"
import { Outlet } from "react-router"

import { useUserStore } from "@/state/user"
import { useChatStore } from "@/state/chat"
import { useChatsListStore } from "@/state/all_chats"

import styles from "./Auth.module.css"
import { useEffect } from "react"

const AuthLayout = () => {
    const authState = useUserStore((state) => state.auth)
    const setUser = useUserStore((state) => state.setUser)
    const setCurrentChat = useChatStore((state) => state.setCurrentChat)
    const setMessages = useChatStore((state) => state.setMessages)
    const setChatsList = useChatsListStore((state) => state.setChatsList)

    useEffect(() => {
        if (authState === true) return
        setUser(null)
        setCurrentChat(null)
        setMessages([])
        setChatsList([])
    }, [authState])

    return (
        <>
            <Gradient />
            <div className={styles.wrapper}>
                <Outlet />
                <div className={styles.bg_image} />
            </div>
        </>
    )
}

export default AuthLayout
