import { Message } from "@/components/UI"
import { IMessage } from "@/types"

import { useUserStore } from "@/state/user"
import { useChatStore } from "@/state/chat"

import styles from "./Chat.module.css"
import { forwardRef } from "react"
import cls from "@/utils/cls"

interface ChatProps {
    className?: string
    ref?: React.Ref<HTMLDivElement>
}

export const Chat = forwardRef<HTMLDivElement, ChatProps>(({ className = "" }, ref) => {
    const currentUser = useUserStore(state => state.data)
    const messages = useChatStore(state => state.messages)

    return (
        <div className={cls(className, styles.chat)} ref={ref}>
            { messages.length > 0 ? (
                <ul className={styles.messages}>
                    { [...messages].reverse().map((item: IMessage, index: number): JSX.Element => (
                        <Message key={index} variant={item.authorId === currentUser?.id ? "right" : "left"}>
                            {item.body}
                        </Message>
                    ))}
                </ul>
            ) : (
                    <div>It's empty</div>
                )
            }
        </div>
    )
})
