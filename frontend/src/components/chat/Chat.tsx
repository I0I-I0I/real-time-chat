import cls from "@/utils/cls"

import { Message } from "@/components/UI"
import { IMessage } from "@/types"

import { useUserStore } from "@/state/user"
import { useChatStore } from "@/state/chat"

import styles from "./Chat.module.css"

interface ChatProps {
    className?: string
}

export const Chat = ({
    className = ""
}: ChatProps) => {
    const currentUser = useUserStore(state => state.data)
    const messages = useChatStore(state => state.messages)

    return (
        <>
            { messages.length > 0 ? (
                <ul className={cls(styles.messages, className)}>
                    { messages.map((item: IMessage, index: number): JSX.Element => (
                        <Message key={index} variant={item.authorId === currentUser?.id ? "right" : "left"}>{item.body}</Message>
                    )) }
                </ul>
            ) : (
                    <div>It's empty</div>
                )
            }
        </>
    )
}
